#include "stdafx.h"
#include "ClangDocHTML.h"
#include "DeclPrinterHTML.h"
#include "MyASTConsumer.h"

/*
void Decl::print(llvm::raw_ostream &Out, unsigned Indentation) const {
print(Out, getASTContext().PrintingPolicy, Indentation);
}

void Decl::print(llvm::raw_ostream &Out, const PrintingPolicy &Policy,
unsigned Indentation) const {
DeclPrinterHTML Printer(Out, getASTContext(), Policy, Indentation);
Printer.Visit(const_cast<Decl*>(this));
}
*/

ClangDocHTML::ClangDocHTML(void)
	: lastDecl(0)
{
	std::string err;
	Out = new llvm::raw_fd_ostream("out.htm", err);
	Out->SetUnbuffered();
	Decl *decl = 0;
	lastComment = commentsByDecl.insert(CommentsByDeclPair(decl, "")); 
}

ClangDocHTML::~ClangDocHTML(void)
{
	Out->close();
	delete Out;
}

void ClangDocHTML::InclusionDirective(SourceLocation HashLoc,
                                const Token &IncludeTok,
                                llvm::StringRef FileName,
                                bool IsAngled,
                                const FileEntry *File,
                                SourceLocation EndLoc,
                                llvm::StringRef SearchPath,
                                llvm::StringRef RelativePath) 
{
	if(!astConsumer->sourceManager->isInSystemHeader(HashLoc))
	{
		std::string str;
		llvm::raw_string_ostream os(str);
		os.SetUnbuffered();
		os << "#include ";
		os << (IsAngled ? "&lt;" : "\"");
		os << FileName;
		os << (IsAngled ? "&gt;" : "\"");
		inclusionDirectives.push_back(str);
	}
}

bool ClangDocHTML::HandleComment(Preprocessor &PP, SourceRange Comment) 
{
	if(Comment.isValid()) 
	{		
		if(astConsumer->sourceManager->isInSystemHeader(Comment.getBegin()))
		{
			return false;
		}
		
		/*
		bool bInvalid = false;
		const char *cbegin = astConsumer->sourceManager->getCharacterData(Comment.getBegin(), &bInvalid);
		const char *cend = astConsumer->sourceManager->getCharacterData(Comment.getEnd(), &bInvalid);
		llvm::StringRef string = llvm::StringRef(cbegin, cend-cbegin);*/

		//Decl *decl = 0;

		/*if(string.startswith("///<"))
		{
			decl = lastDecl;
			decl->print(*Out);
		}*/

		commentsList.push_back(Comment);
		//lastComment = commentsByDecl.insert(CommentsByDeclPair(decl, string) ); 
		
		//*Out << string << "<br/>\n";
	}
	return false;
}

void ClangDocHTML::HandleTranslationUnit(TranslationUnitDecl *D) {

	
	for(CommentsList::iterator c=commentsList.begin(); c!=commentsList.end(); ++c) {
		
		bool bInvalid = false;
		const char *cbegin = astConsumer->sourceManager->getCharacterData((*c).getBegin(), &bInvalid);
		const char *cend = astConsumer->sourceManager->getCharacterData((*c).getEnd(), &bInvalid);
		llvm::StringRef string = llvm::StringRef(cbegin, cend-cbegin);

		if(string.startswith("///< ")) {
			for(DeclList::reverse_iterator d=declList.rbegin(); d!=declList.rend(); ++d) {
				SourceLocation dl = (*d)->getLocation();
				SourceLocation cl = (*c).getEnd();
				if(cl < dl) {
					continue;
				} else {
					commentsByDecl.insert(CommentsByDeclPair((*d), string.substr(5)) ); 
					break;
				}
			}
		}
		else if(string.startswith("/// ")) {
			for(DeclList::iterator d=declList.begin(); d!=declList.end(); ++d) {
				SourceLocation dl = (*d)->getLocation();
				SourceLocation cl = (*c).getEnd();
				if(dl < cl) {
					continue;
				} else {
					commentsByDecl.insert(CommentsByDeclPair((*d), string.substr(4)) ); 
					break;
				}
			}
		}
	}

	for (DeclByType::iterator it=declByType.begin(); it!=declByType.end(); ++it) {
		*Out << (it->second)->getKind() << " " << (it->second)->getDeclKindName() << "\n";
	}

	*Out << "<table border = \"1\">\n";

	if(!inclusionDirectives.empty())
	{
		*Out << "<th align=\"left\">" << "Include Directives" << "</th>\n";
		
		*Out << "<tbody>";

		for(InclusionDirectivesIt it = inclusionDirectives.begin(); it != inclusionDirectives.end(); ++it) 
		{	
			*Out << "<tr><td align=\"left\">\n";
			*Out << *it;
			*Out << "</td></tr>\n";
		}
		
		*Out << "</tbody>";
	}

	

	/*PreprocessingRecord *PPRec = astConsumer->preprocessor->getPreprocessingRecord();
	if (PPRec && PPRec->begin() != PPRec->end()) {

		for (PreprocessingRecord::iterator E = PPRec->begin(), EEnd = PPRec->end(); E != EEnd; ++E) {

			if (MacroDefinition *MD = dyn_cast<MacroDefinition>(*E)) {
				
				if (InclusionDirective *ID = dyn_cast<InclusionDirective>(*E)) {
					
					*Out << "<tr><td align=\"left\">\n";
					*Out << ID->getFileName();
					*Out << "</td></tr>\n";
				}
			}
		}
	}*/


	
    for (DeclByTypeIt it=declByType.begin(); it!=declByType.end();) {
		
		DeclByTypeItPair result = declByType.equal_range(it->first);
				
		*Out << "<th align=\"left\">" << (it->second)->getDeclKindName() << "</th>\n";
		
		*Out << "<tbody>";

		for (it=result.first; it!=result.second; ++it) {
			int Indentation = 0;
		*Out << "<tr>";
			Decl *decl = it->second;
			{
				*Out << "<td align=\"right\">\n";

				*Out << "<pre>\t";
				PrintingPolicyHTML printingPolicy = PrintingPolicyHTML(decl->getASTContext().PrintingPolicy);
				printingPolicy.htmlSuppressIdentifier = 1;
				printingPolicy.SuppressInitializers = 1;
				DeclPrinterHTML Printer(*Out, decl->getASTContext(), printingPolicy, Indentation);
				Printer.Visit(decl);
				*Out << "</pre>\n";
				*Out << "</td>\n";
			}

			{
				*Out << "<td>\n";

				*Out << "<pre>";
				PrintingPolicyHTML printingPolicy = PrintingPolicyHTML(decl->getASTContext().PrintingPolicy);
				printingPolicy.SuppressSpecifiers = 1;
				DeclPrinterHTML Printer(*Out, decl->getASTContext(), printingPolicy, Indentation);
				Printer.Visit(decl);
				*Out << "</pre>\n";
				*Out << "</td>\n";
			}
			
			{
				*Out << "<td>\n";

				*Out << "<pre>";
				CommentsByDeclItPair comments = commentsByDecl.equal_range(decl);
				
				for (CommentsByDeclIt cit=comments.first; cit!=comments.second; ++cit) {

					*Out << cit->second << " ";
				}

				//SourceLocation sl = decl->getLocation();
				//SourceLocation slend = astConsumer->preprocessor->getCurrentLexer()

				//astConsumer->sourceManager->get
		//const char *cbegin = astConsumer->sourceManager->getCharacterData(Comment.getBegin(), &bInvalid);
				*Out << "</pre>\n";
				*Out << "</td>\n";
			}
		*Out << "</tr>";
		}
		
		*Out << "</tbody>";
		
	}
		*Out << "</table>\n";
}

void ClangDocHTML::HandleTopLevelDecl(Decl *D) {

	declList.push_back(D);

	// Skip template function specializations
	if( isa<FunctionDecl>(D) && cast<FunctionDecl>(D)->getTemplateSpecializationInfo() ) {
		return;
	}

	// Skip CXXRecord
	if(D->getKind() == Decl::CXXRecord) {
		return;
	}
	
	/*lastDecl = D;
			lastDecl->print(*Out);
			*Out << " \n";
	
	if(lastComment != commentsByDecl.end() && lastComment->second.startswith("///")) 
	{
		if(lastDecl) {
			//*Out << lastComment->second;
			//*Out << "<br/>";

			CommentsByDeclPair p = CommentsByDeclPair(lastDecl, lastComment->second);
			commentsByDecl.erase(lastComment);
			commentsByDecl.insert(p);
			lastComment = commentsByDecl.end();
		
		}
	}*/

	
	declByType.insert ( std::pair<int,Decl *>(D->getKind(), D) ); 
}


void ClangDocHTML::VisitVarDecl(VarDecl *D) {


}