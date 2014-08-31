#include "stdafx.h"

#include "MyASTConsumer.h"

MyASTConsumer::MyASTConsumer() : ASTConsumer() {

	mutationListener = new MyASTMutationListener;
}

MyASTConsumer::~MyASTConsumer() {
	//@todo: delete MyASTMutationListener?
}

/// Initialize - This is called to initialize the consumer, providing the
/// ASTContext.
void MyASTConsumer::Initialize(ASTContext &Context) {

}

/// HandleTopLevelDecl - Handle the specified top-level declaration.  This is
/// called by the parser to process every top-level Decl*. Note that D can be
/// the head of a chain of Decls (e.g. for `int a, b` the chain will have two
/// elements). Use Decl::getNextDeclarator() to walk the chain.
void MyASTConsumer::HandleTopLevelDecl(DeclGroupRef D) {

	static int count = 0;
	for(DeclGroupRef::iterator it = D.begin(); it != D.end(); ++it)
	{
		Decl *decl = *it;
		if(decl) {
			if(sourceManager->isInSystemHeader(decl->getLocation())) {
				continue;
			}
			std::cerr << std::endl << "HandleTopLevelDecl" << std::endl;
			decl->dump();
		//decl->dumpXML();
			std::cerr << std::endl << decl << std::endl;
			html->HandleTopLevelDecl(decl);
		}
		/*decl->dumpXML();
		std::cerr  << std::endl << decl << std::endl;*/
		/*count++;
		VarDecl *vd = dyn_cast<VarDecl>(*it);
		if(!vd)
		{
		continue;
		}
		std::cerr << vd << std::endl;
		if( vd->isFileVarDecl() && vd->hasExternalStorage() )
		{
		std::cerr << "Read top-level variable decl: '";
		std::cerr << vd->getDeclName().getAsString() ;
		std::cerr << std::endl;
		}*/
	}
}

/// HandleInterestingDecl - Handle the specified interesting declaration. This
/// is called by the AST reader when deserializing things that might interest
/// the consumer. The default implementation forwards to HandleTopLevelDecl.
void MyASTConsumer::HandleInterestingDecl(DeclGroupRef D) {

	for(DeclGroupRef::iterator it = D.begin(); it != D.end(); ++it)
	{
		Decl *decl = *it;
		if(decl) {
			std::cerr << std::endl << "HandleInterestingDecl" << std::endl;
			decl->dump();
			std::cerr << std::endl << decl << std::endl;
		}
	}
}

/// HandleTranslationUnit - This method is called when the ASTs for entire
/// translation unit have been parsed.
void MyASTConsumer::HandleTranslationUnit(ASTContext &Ctx) {
	TranslationUnitDecl *decl = Ctx.getTranslationUnitDecl();
	if(decl) {
		/*if(decl->getLocation().isInvalid() || sourceManager->isInSystemHeader(decl->getLocation())) {
				return;
			}*/
		std::cerr << std::endl << "HandleTranslationUnit" << std::endl;
		//decl->dump();
		std::cerr << std::endl << decl << std::endl;
		html->HandleTranslationUnit(decl);
	}
}

/// HandleTagDeclDefinition - This callback is invoked each time a TagDecl
/// (e.g. struct, union, enum, class) is completed.  This allows the client to
/// hack on the type, which can occur at any point in the file (because these
/// can be defined in declspecs).
void MyASTConsumer::HandleTagDeclDefinition(TagDecl *D) {
	TagDecl *decl = D;
	if(decl) {
		if(decl->getLocation().isInvalid() || sourceManager->isInSystemHeader(decl->getLocation())) {
				return;
			}
		std::cerr << std::endl << "HandleTagDeclDefinition" << std::endl;
		decl->dump();
		std::cerr << std::endl << decl << std::endl;
	}

}

/// CompleteTentativeDefinition - Callback invoked at the end of a translation
/// unit to notify the consumer that the given tentative definition should be
/// completed.
///
/// The variable declaration itself will be a tentative
/// definition. If it had an incomplete array type, its type will
/// have already been changed to an array of size 1. However, the
/// declaration remains a tentative definition and has not been
/// modified by the introduction of an implicit zero initializer.
void MyASTConsumer::CompleteTentativeDefinition(VarDecl *D) {
	VarDecl *decl = D;
	if(decl) {
		std::cerr << std::endl << "CompleteTentativeDefinition" << std::endl;
		decl->dump();
		std::cerr << std::endl << decl << std::endl;
	}

}

/// \brief Callback involved at the end of a translation unit to
/// notify the consumer that a vtable for the given C++ class is
/// required.
///
/// \param RD The class whose vtable was used.
///
/// \param DefinitionRequired Whether a definition of this vtable is
/// required in this translation unit; otherwise, it is only needed if
/// it was actually used.
void MyASTConsumer::HandleVTable(CXXRecordDecl *RD, bool DefinitionRequired) {
	CXXRecordDecl *decl = RD;
	if(decl) {
		std::cerr << std::endl << "HandleVTable" << std::endl;
		decl->dump();
		std::cerr << std::endl << decl << std::endl;
	}

}

/// \brief If the consumer is interested in entities getting modified after
/// their initial creation, it should return a pointer to
/// an ASTMutationListener here.
ASTMutationListener *MyASTConsumer::GetASTMutationListener() { 
	return mutationListener; 
}

/// \brief If the consumer is interested in entities being deserialized from
/// AST files, it should return a pointer to a ASTDeserializationListener here
ASTDeserializationListener *MyASTConsumer::GetASTDeserializationListener() { 
	return 0; 
}

/// PrintStats - If desired, print any statistics.
void MyASTConsumer::PrintStats() {

}

