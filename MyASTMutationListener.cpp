#include "stdafx.h"
#include "MyASTMutationListener.h"


/// \brief A new TagDecl definition was completed.
void MyASTMutationListener::CompletedTagDefinition(const TagDecl *D) {	
	const TagDecl *decl = D;
	if(decl) {
		std::cerr << std::endl << "CompletedTagDefinition" << std::endl;
		decl->dump();
		std::cerr << std::endl << decl << std::endl;
	}
}

/// \brief A new declaration with name has been added to a DeclContext.
void MyASTMutationListener::AddedVisibleDecl(const DeclContext *DC, const Decl *D) {
	std::cerr << std::endl << "AddedVisibleDecl" << std::endl;
	if(DC) {
		DC->dumpDeclContext();
		std::cerr << std::endl;
	}

	const Decl *decl = D;
	if(decl) {
		decl->dump();
		std::cerr << std::endl << decl << std::endl;
	}
}

/// \brief An implicit member was added after the definition was completed.
void MyASTMutationListener::AddedCXXImplicitMember(const CXXRecordDecl *RD, const Decl *D) {
	std::cerr << std::endl << "AddedCXXImplicitMember" << std::endl;
	if(RD) {
		RD->dump();
		std::cerr << std::endl;
	}

	const Decl *decl = D;
	if(decl) {
		decl->dump();
		std::cerr << std::endl << decl << std::endl;
	}
}

/// \brief A template specialization (or partial one) was added to the
/// template declaration.
void MyASTMutationListener::AddedCXXTemplateSpecialization(const ClassTemplateDecl *TD,
	const ClassTemplateSpecializationDecl *D) {
	std::cerr << std::endl << "AddedCXXTemplateSpecialization" << std::endl;
	if(TD) {
		TD->dump();
		std::cerr << std::endl;
	}

	const Decl *decl = D;
	if(decl) {
		decl->dump();
		std::cerr << std::endl << decl << std::endl;
	}
}

/// \brief A template specialization (or partial one) was added to the
/// template declaration.
void MyASTMutationListener::AddedCXXTemplateSpecialization(const FunctionTemplateDecl *TD,
	const FunctionDecl *D) {
	std::cerr << std::endl << "AddedCXXTemplateSpecialization2" << std::endl;
	if(TD) {
		TD->dump();
		std::cerr << std::endl;
	}

	const Decl *decl = D;
	if(decl) {
		decl->dump();
		std::cerr << std::endl << decl << std::endl;
	}
}

/// \brief An implicit member got a definition.
void MyASTMutationListener::CompletedImplicitDefinition(const FunctionDecl *D) {
	const FunctionDecl *decl = D;
	if(decl) {
		std::cerr << std::endl << "CompletedImplicitDefinition" << std::endl;
		decl->dump();
		std::cerr << std::endl << decl << std::endl;
	}
}

/// \brief A static data member was implicitly instantiated.
void MyASTMutationListener::StaticDataMemberInstantiated(const VarDecl *D) {
	const VarDecl *decl = D;
	if(decl) {
		std::cerr << std::endl << "StaticDataMemberInstantiated" << std::endl;
		decl->dump();
		std::cerr << std::endl << decl << std::endl;
	}
}
