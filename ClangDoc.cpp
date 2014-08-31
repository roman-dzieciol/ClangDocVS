// ClangDoc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MyASTConsumer.h"



// This function isn't referenced outside its translation unit, but it
// can't use the "static" keyword because its address is used for
// GetMainExecutable (since some platforms don't support taking the
// address of main, and some platforms can't implement GetMainExecutable
// without being given the address of a function in the main executable).
llvm::sys::Path GetExecutablePath(const char *Argv0) {
	// This just needs to be some symbol in the binary; C++ doesn't
	// allow taking the address of ::main however.
	void *MainAddr = (void*) (intptr_t) GetExecutablePath;
	return llvm::sys::Path::GetMainExecutable(Argv0, MainAddr);
}

static int Execute(llvm::Module *Mod, char * const *envp) {
	llvm::InitializeNativeTarget();

	std::string Error;
	llvm::OwningPtr<llvm::ExecutionEngine> EE(
		llvm::ExecutionEngine::createJIT(Mod, &Error));
	if (!EE) {
		llvm::errs() << "unable to make execution engine: " << Error << "\n";
		return 255;
	}

	llvm::Function *EntryFn = Mod->getFunction("main");
	if (!EntryFn) {
		llvm::errs() << "'main' function not found in module.\n";
		return 255;
	}

	// FIXME: Support passing arguments.
	std::vector<std::string> Args;
	Args.push_back(Mod->getModuleIdentifier());

	return EE->runFunctionAsMain(EntryFn, Args, envp);
}

int main(int argc, const char **argv, char * const *envp) 
{
	// Path
	void *MainAddr = (void*) (intptr_t) GetExecutablePath;
	llvm::sys::Path Path = GetExecutablePath(argv[0]);

	// DiagnosticOptions
	DiagnosticOptions diagnosticOptions;

	// DiagnosticClient
	TextDiagnosticPrinter *DiagClient = new TextDiagnosticPrinter(llvm::outs(), diagnosticOptions);

	// Diagnostic
	llvm::IntrusiveRefCntPtr<DiagnosticIDs> DiagID(new DiagnosticIDs());
	Diagnostic diagnostic(DiagID, DiagClient);


	//DiagnosticOptions DiagOpts;
	//llvm::IntrusiveRefCntPtr<Diagnostic> diagnostic = CompilerInstance::createDiagnostics(DiagOpts, argc, argv);

	// LangOptions
	LangOptions langOptions;
	langOptions.CPlusPlus = 1;
	langOptions.CPlusPlus0x = 1;
	langOptions.Microsoft = 1;
	langOptions.Bool = 1;
	langOptions.Exceptions = 1;
	langOptions.CXXExceptions = 1;
	langOptions.EmitAllDecls = 1;

	// FileManager
	FileSystemOptions fileSystemOptions;
	FileManager fileManager(fileSystemOptions);

	// SourceManager
	SourceManager sourceManager(diagnostic, fileManager);

	// HeadderSearch
	HeaderSearch headerSearch(fileManager);

	// TargetOptions
	TargetOptions targetOptions;
	targetOptions.Triple = llvm::sys::getHostTriple();

	// TargetInfo
	TargetInfo *pTargetInfo = 
		TargetInfo::CreateTargetInfo(
		diagnostic,
		targetOptions);

	// HeaderSearchOptions
	HeaderSearchOptions headerSearchOptions;
	ApplyHeaderSearchOptions(
		headerSearch,
		headerSearchOptions,
		langOptions,
		pTargetInfo->getTriple());

	// Preprocessor
	Preprocessor preprocessor(
		diagnostic,
		langOptions,
		*pTargetInfo,
		sourceManager,
		headerSearch);

	// PreprocessorOptions
	PreprocessorOptions preprocessorOptions;
	preprocessorOptions.DetailedRecord = true;
    preprocessor.createPreprocessingRecord();

	// FrontendOptions
	FrontendOptions frontendOptions;

	
	// InitializePreprocessor
	InitializePreprocessor(
		preprocessor,
		preprocessorOptions,
		headerSearchOptions,
		frontendOptions);
	
	//preprocessor.SetCommentRetentionState(true, true);

	// Tutorial
	const FileEntry *pFile = fileManager.getFile(
		"test.cpp",
		true);
	sourceManager.createMainFileID(pFile);


	/*preprocessor.EnterMainSourceFile();
	Token Tok;
	do {
		preprocessor.Lex(Tok);  // read one token
		//if (context.diags.hasErrorOccurred())  // stop lexing/pp on error
		//	break;
		preprocessor.DumpToken(Tok);  // outputs to cerr
		std::cerr << std::endl;
	} while (Tok.isNot(tok::eof));*/


	const TargetInfo &targetInfo = *pTargetInfo;

	IdentifierTable identifierTable(langOptions);
	SelectorTable selectorTable;

	Builtin::Context builtinContext(targetInfo);
	ASTContext astContext(
		langOptions,
		sourceManager,
		targetInfo,
		identifierTable,
		selectorTable,
		builtinContext,
		0 /* size_reserve*/);
	// ASTConsumer astConsumer;
	MyASTConsumer astConsumer;
	astConsumer.sourceManager = &sourceManager;
	astConsumer.html = new ClangDocHTML;
	astConsumer.html->astConsumer = &astConsumer;
	preprocessor.addPPCallbacks(astConsumer.html);
	preprocessor.AddCommentHandler(astConsumer.html);

	Sema sema(
		preprocessor,
		astContext,
		astConsumer);
	sema.Initialize();

	//MySemanticAnalisys mySema( preprocessor, astContext, astConsumer);

	//Parser parser( preprocessor, sema);
	//parser.ParseTranslationUnit();
	astConsumer.preprocessor = &sema.PP;
	ParseAST(preprocessor, &astConsumer, astContext); 
	return 0;
}