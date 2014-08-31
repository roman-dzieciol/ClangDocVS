#pragma once

class MyASTMutationListener : public ASTMutationListener {

	/// \brief A new TagDecl definition was completed.
	virtual void CompletedTagDefinition(const TagDecl *D);

	/// \brief A new declaration with name has been added to a DeclContext.
	virtual void AddedVisibleDecl(const DeclContext *DC, const Decl *D);

	/// \brief An implicit member was added after the definition was completed.
	virtual void AddedCXXImplicitMember(const CXXRecordDecl *RD, const Decl *D);

	/// \brief A template specialization (or partial one) was added to the
	/// template declaration.
	virtual void AddedCXXTemplateSpecialization(const ClassTemplateDecl *TD,
		const ClassTemplateSpecializationDecl *D);

	/// \brief A template specialization (or partial one) was added to the
	/// template declaration.
	virtual void AddedCXXTemplateSpecialization(const FunctionTemplateDecl *TD,
		const FunctionDecl *D);

	/// \brief An implicit member got a definition.
	virtual void CompletedImplicitDefinition(const FunctionDecl *D);

	/// \brief A static data member was implicitly instantiated.
	virtual void StaticDataMemberInstantiated(const VarDecl *D);

};