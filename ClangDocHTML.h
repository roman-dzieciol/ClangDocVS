#pragma once

class MyASTConsumer;

class ClangDocHTML : 
	public DeclVisitor<ClangDocHTML>,
	public PPCallbacks,
	public CommentHandler
{
public:
	llvm::raw_fd_ostream *Out;
	typedef std::multimap<int, Decl*> DeclByType;
	typedef DeclByType::iterator DeclByTypeIt;
	typedef std::pair<DeclByTypeIt,DeclByTypeIt> DeclByTypeItPair;
	DeclByType declByType;
	Decl *lastDecl;

	typedef std::vector<std::string> InclusionDirectives;
	typedef InclusionDirectives::iterator InclusionDirectivesIt;
	typedef std::pair<InclusionDirectivesIt,InclusionDirectivesIt> InclusionDirectivesItPair;
	InclusionDirectives inclusionDirectives;

	
	typedef std::multimap<Decl *, llvm::StringRef> CommentsByDecl;
	typedef std::pair<Decl *, llvm::StringRef> CommentsByDeclPair;
	typedef CommentsByDecl::iterator CommentsByDeclIt;
	typedef std::pair<CommentsByDeclIt,CommentsByDeclIt> CommentsByDeclItPair;
	CommentsByDecl commentsByDecl;
	CommentsByDeclIt lastComment;

	
	typedef std::vector<SourceRange> CommentsList;
	CommentsList commentsList;

	typedef std::vector<Decl *> DeclList;
	DeclList declList;

	MyASTConsumer *astConsumer;

public:
	ClangDocHTML(void);
	virtual ~ClangDocHTML(void);

	
	void HandleTranslationUnit(TranslationUnitDecl *D);
	void HandleTopLevelDecl(Decl *D);
	
    void VisitVarDecl(VarDecl *D);

	void PrintSection(DeclByTypeItPair result);

	
  /// \brief This callback is invoked whenever an inclusion directive of
  /// any kind (\c #include, \c #import, etc.) has been processed, regardless
  /// of whether the inclusion will actually result in an inclusion.
  ///
  /// \param HashLoc The location of the '#' that starts the inclusion 
  /// directive.
  ///
  /// \param IncludeTok The token that indicates the kind of inclusion 
  /// directive, e.g., 'include' or 'import'.
  ///
  /// \param FileName The name of the file being included, as written in the 
  /// source code.
  ///
  /// \param IsAngled Whether the file name was enclosed in angle brackets;
  /// otherwise, it was enclosed in quotes.
  ///
  /// \param File The actual file that may be included by this inclusion 
  /// directive.
  ///
  /// \param EndLoc The location of the last token within the inclusion
  /// directive.
  ///
  /// \param SearchPath Contains the search path which was used to find the file
  /// in the file system. If the file was found via an absolute include path,
  /// SearchPath will be empty. For framework includes, the SearchPath and
  /// RelativePath will be split up. For example, if an include of "Some/Some.h"
  /// is found via the framework path
  /// "path/to/Frameworks/Some.framework/Headers/Some.h", SearchPath will be
  /// "path/to/Frameworks/Some.framework/Headers" and RelativePath will be
  /// "Some.h".
  ///
  /// \param RelativePath The path relative to SearchPath, at which the include
  /// file was found. This is equal to FileName except for framework includes.
  virtual void InclusionDirective(SourceLocation HashLoc,
                                  const Token &IncludeTok,
                                  llvm::StringRef FileName,
                                  bool IsAngled,
                                  const FileEntry *File,
                                  SourceLocation EndLoc,
                                  llvm::StringRef SearchPath,
                                  llvm::StringRef RelativePath);

  

  // The handler shall return true if it has pushed any tokens
  // to be read using e.g. EnterToken or EnterTokenStream.
  virtual bool HandleComment(Preprocessor &PP, SourceRange Comment);
};

