#pragma once

#include "MyASTMutationListener.h"
#include "ClangDocHTML.h"

class MyASTConsumer : public ASTConsumer
{
private:
	MyASTMutationListener *mutationListener;

public:
	SourceManager *sourceManager;
	Preprocessor *preprocessor;
	ClangDocHTML *html;

public:
	MyASTConsumer();
	virtual ~MyASTConsumer();


  /// Initialize - This is called to initialize the consumer, providing the
  /// ASTContext.
  virtual void Initialize(ASTContext &Context);

  /// HandleTopLevelDecl - Handle the specified top-level declaration.  This is
  /// called by the parser to process every top-level Decl*. Note that D can be
  /// the head of a chain of Decls (e.g. for `int a, b` the chain will have two
  /// elements). Use Decl::getNextDeclarator() to walk the chain.
  virtual void HandleTopLevelDecl(DeclGroupRef D);

  /// HandleInterestingDecl - Handle the specified interesting declaration. This
  /// is called by the AST reader when deserializing things that might interest
  /// the consumer. The default implementation forwards to HandleTopLevelDecl.
  virtual void HandleInterestingDecl(DeclGroupRef D);

  /// HandleTranslationUnit - This method is called when the ASTs for entire
  /// translation unit have been parsed.
  virtual void HandleTranslationUnit(ASTContext &Ctx);

  /// HandleTagDeclDefinition - This callback is invoked each time a TagDecl
  /// (e.g. struct, union, enum, class) is completed.  This allows the client to
  /// hack on the type, which can occur at any point in the file (because these
  /// can be defined in declspecs).
  virtual void HandleTagDeclDefinition(TagDecl *D);

  /// CompleteTentativeDefinition - Callback invoked at the end of a translation
  /// unit to notify the consumer that the given tentative definition should be
  /// completed.
  ///
  /// The variable declaration itself will be a tentative
  /// definition. If it had an incomplete array type, its type will
  /// have already been changed to an array of size 1. However, the
  /// declaration remains a tentative definition and has not been
  /// modified by the introduction of an implicit zero initializer.
  virtual void CompleteTentativeDefinition(VarDecl *D);

  /// \brief Callback involved at the end of a translation unit to
  /// notify the consumer that a vtable for the given C++ class is
  /// required.
  ///
  /// \param RD The class whose vtable was used.
  ///
  /// \param DefinitionRequired Whether a definition of this vtable is
  /// required in this translation unit; otherwise, it is only needed if
  /// it was actually used.
  virtual void HandleVTable(CXXRecordDecl *RD, bool DefinitionRequired);

  /// \brief If the consumer is interested in entities getting modified after
  /// their initial creation, it should return a pointer to
  /// an ASTMutationListener here.
  virtual ASTMutationListener *GetASTMutationListener();

  /// \brief If the consumer is interested in entities being deserialized from
  /// AST files, it should return a pointer to a ASTDeserializationListener here
  virtual ASTDeserializationListener *GetASTDeserializationListener();

  /// PrintStats - If desired, print any statistics.
  virtual void PrintStats();
};