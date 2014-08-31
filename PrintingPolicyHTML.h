#pragma once

class PrintingPolicyHTML : public PrintingPolicy {
public:
  PrintingPolicyHTML(const LangOptions &LO) 
	  : PrintingPolicy(LO)
	  , htmlSuppressLeftColumn(false)
	  , htmlSuppressRightColumn(false)
	  , htmlSuppressIdentifier(false)
  {}
  
  PrintingPolicyHTML(const PrintingPolicy &rhs) 
	  : PrintingPolicy(rhs.LangOpts) 
	  , htmlSuppressLeftColumn(false)
	  , htmlSuppressRightColumn(false)
	  , htmlSuppressIdentifier(false)
  {}
  
  bool htmlSuppressLeftColumn : 1;
  bool htmlSuppressRightColumn : 1;
  bool htmlSuppressIdentifier : 1;

};
