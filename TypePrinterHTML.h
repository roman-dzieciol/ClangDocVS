#pragma once

#include "PrintingPolicyHTML.h"

class TypePrinterHTML {
	PrintingPolicyHTML Policy;

public:
	explicit TypePrinterHTML(const PrintingPolicyHTML &Policy) : Policy(Policy) { }

	void print(const Type *ty, Qualifiers qs, std::string &buffer);
	void print(QualType T, std::string &S);
	void AppendScope(DeclContext *DC, std::string &S);
	void printTag(TagDecl *T, std::string &S);
#define ABSTRACT_TYPE(CLASS, PARENT)
#define TYPE(CLASS, PARENT) \
	void print##CLASS(const CLASS##Type *T, std::string &S);
#include "clang/AST/TypeNodes.def"
};
