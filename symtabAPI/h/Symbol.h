/*
 * Copyright (c) 1996-2007 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/************************************************************************
 * $Id: Symbol.h,v 1.20 2008/11/03 15:19:24 jaw Exp $
 * Symbol.h: symbol table objects.
************************************************************************/

#if !defined(_Symbol_h_)
#define _Symbol_h_


/************************************************************************
 * header files.
************************************************************************/

#include "symutil.h"
#include "Collections.h"
#include "Type.h"

#include "Annotatable.h"
#include "Serialization.h"

#ifndef CASE_RETURN_STR
#define CASE_RETURN_STR(x) case x: return #x
#endif


#if 0
typedef struct {} symbol_file_name_a;
typedef struct {} symbol_version_names_a;
typedef struct {} symbol_variables_a;
typedef struct {} symbol_parameters_a;
#endif

namespace Dyninst{
namespace SymtabAPI{

class Module;
class typeCommon;
class localVarCollection;
class Region;
class Function;
 class Variable;

/************************************************************************
 * class Symbol
************************************************************************/

class Symbol : public Serializable,
               public AnnotatableSparse 
{
   friend class typeCommon;
   friend class Symtab;

   friend std::string parseStabString(Module *, int linenum, char *, int, 
         typeCommon *);

   public:

   enum SymbolType {
      ST_UNKNOWN,
      ST_FUNCTION,
      ST_OBJECT,
      ST_MODULE,
      ST_NOTYPE
   };

   static const char *symbolType2Str(SymbolType t);

   enum SymbolLinkage {
      SL_UNKNOWN,
      SL_GLOBAL,
      SL_LOCAL,
      SL_WEAK
   };

   static const char *symbolLinkage2Str(SymbolLinkage t);

   enum SymbolTag {
      TAG_UNKNOWN,
      TAG_USER,
      TAG_LIBRARY,
      TAG_INTERNAL
   };

   static const char *symbolTag2Str(SymbolTag t);

   SYMTABEXPORT Symbol (); // note: this ctor is called surprisingly often!
   SYMTABEXPORT Symbol (unsigned);
   SYMTABEXPORT Symbol (const std::string name,const std::string modulename, SymbolType, SymbolLinkage,
         Offset, Region *sec = NULL, unsigned size = 0, bool isInDynsymtab_ = false, 
         bool isInSymtab_ = true, bool isAbsolute_ = false);
   SYMTABEXPORT Symbol (const std::string name,Module *module, SymbolType, SymbolLinkage,
         Offset, Region *sec = NULL, unsigned size = 0, bool isInDynsymtab_ = false,
         bool isInSymtab_ = true, bool isAbsolute_ = false);
   SYMTABEXPORT Symbol (const Symbol &);
   SYMTABEXPORT ~Symbol();

   SYMTABEXPORT Symbol&        operator= (const Symbol &);
   SYMTABEXPORT bool          operator== (const Symbol &) const;

   SYMTABEXPORT const std::string&getModuleName ()        const;
   SYMTABEXPORT Module*	        getModule()		        const; 
   SYMTABEXPORT SymbolType        getType ()              const;
   SYMTABEXPORT SymbolLinkage     getLinkage ()           const;
   SYMTABEXPORT Offset            getAddr ()              const;
   SYMTABEXPORT Region		    *getSec ()      	    const;
   SYMTABEXPORT bool              isInDynSymtab()         const;
   SYMTABEXPORT bool              isInSymtab()            const;
   SYMTABEXPORT bool              isAbsolute()            const;

   SYMTABEXPORT bool              isFunction()            const;
   SYMTABEXPORT bool              setFunction(Function * func);
   SYMTABEXPORT Function *        getFunction()           const;

   SYMTABEXPORT bool              isVariable()            const;
   SYMTABEXPORT bool              setVariable(Variable *var);
   SYMTABEXPORT Variable *        getVariable()           const;

   /***********************************************************
     Name Output Functions
    ***********************************************************/		
   SYMTABEXPORT const std::string&      getMangledName ()              const;
   SYMTABEXPORT const std::string&	     getPrettyName()       	const;
   SYMTABEXPORT const std::string&      getTypedName() 		const;

   /* Deprecated */
   SYMTABEXPORT const std::string &getName() const { return getMangledName(); }

   SYMTABEXPORT bool setAddr (Offset newAddr);

   SYMTABEXPORT bool setSymbolType(SymbolType sType);

   SYMTABEXPORT unsigned            getSize ()               const;
   SYMTABEXPORT SymbolTag            tag ()               const;
   SYMTABEXPORT bool	setSize(unsigned ns);
   SYMTABEXPORT bool	setModuleName(std::string module);
   SYMTABEXPORT bool 	setModule(Module *mod);
   SYMTABEXPORT bool  setDynSymtab();
   SYMTABEXPORT bool  clearDynSymtab();
   SYMTABEXPORT bool  setIsInSymtab();
   SYMTABEXPORT bool  clearIsInSymtab();
   SYMTABEXPORT bool  setIsAbsolute();
   SYMTABEXPORT bool  clearIsAbsolute();

   SYMTABEXPORT bool  setVersionFileName(std::string &fileName);
   SYMTABEXPORT bool  setVersions(std::vector<std::string> &vers);
   SYMTABEXPORT bool  setVersionNum(unsigned verNum);
   SYMTABEXPORT bool  getVersionFileName(std::string &fileName);
   SYMTABEXPORT bool  getVersions(std::vector<std::string> *&vers);
   SYMTABEXPORT bool  VersionNum(unsigned &verNum);

   friend
      std::ostream& operator<< (std::ostream &os, const Symbol &s);

   public:
   static std::string emptyString;


   private:
   void setPrettyName(std::string pN) { prettyName_ = pN; };
   void setTypedName(std::string tN) { typedName_ = tN; };

   Module*       module_;
   SymbolType    type_;
   SymbolLinkage linkage_;
   Offset        addr_;
   Region*      sec_;
   unsigned      size_;  // size of this symbol. This is NOT available on all platforms.
   bool          isInDynsymtab_;
   bool          isInSymtab_;
   bool          isAbsolute_;

   Function*     function_;  // if this symbol represents a function, this is a pointer
                             // to the corresponding Function object
   Variable     *variable_;  // Should combine into an "Aggregate" parent class...

   std::string mangledName_;
   std::string prettyName_;
   std::string typedName_;

   SymbolTag     tag_;
   int           framePtrRegNum_;
   Type          *retType_;
   // Module Objects are created in Symtab and not in Object.
   // So we need a way to store the name of the module in 
   // which the symbol is present.
   std::string moduleName_;  
   std::string fileName_;

#if !defined (USE_ANNOTATIONS)
   std::vector<std::string> verNames_;
#endif

   public:
   SYMTABEXPORT void serialize(SerializerBase *, const char *tag = "Symbol");
};

inline
Symbol::Symbol(unsigned)
   : //name_("*bad-symbol*"), module_("*bad-module*"),
   module_(NULL), type_(ST_UNKNOWN), linkage_(SL_UNKNOWN), addr_(0), sec_(NULL), size_(0), 
   isInDynsymtab_(false), isInSymtab_(true), isAbsolute_(false), tag_(TAG_UNKNOWN),
   retType_(NULL), moduleName_("")
   //vars_(NULL), params_(NULL) 
{
}
#if 0
   inline
Symbol::Symbol(unsigned)
   : //name_("*bad-symbol*"), module_("*bad-module*"),
   module_(NULL), type_(ST_UNKNOWN), linkage_(SL_UNKNOWN), addr_(0), sec_(NULL), size_(0), upPtr_(NULL),
   isInDynsymtab_(false), isInSymtab_(true), tag_(TAG_UNKNOWN), retType_(NULL), moduleName_(""), 
   vars_(NULL), params_(NULL) {
   }
#endif

inline
bool
Symbol::operator==(const Symbol& s) const 
{
   // explicitly ignore tags when comparing symbols
   return ((module_  == s.module_)
         && (type_    == s.type_)
         && (linkage_ == s.linkage_)
         && (addr_    == s.addr_)
         && (sec_     == s.sec_)
         && (size_    == s.size_)
#if 0
         && (upPtr_    == s.upPtr_)
#endif
         && (isInDynsymtab_ == s.isInDynsymtab_)
         && (isInSymtab_ == s.isInSymtab_)
         && (isAbsolute_ == s.isAbsolute_)
         && (retType_    == s.retType_)
         && (mangledName_ == s.mangledName_)
         && (prettyName_ == s.prettyName_)
         && (typedName_ == s.typedName_)
         && (moduleName_ == s.moduleName_));
}

class LookupInterface 
{
   public:
      SYMTABEXPORT LookupInterface();
      SYMTABEXPORT virtual bool getAllSymbolsByType(std::vector<Symbol *> &ret,
            Symbol::SymbolType sType) = 0;
      SYMTABEXPORT virtual bool findSymbolByType(std::vector<Symbol *> &ret,
            const std::string name,
            Symbol::SymbolType sType,
            bool isMangled = false,
            bool isRegex = false,
            bool checkCase = false) = 0;
      SYMTABEXPORT virtual bool findType(Type *&type, std::string name) = 0;
      SYMTABEXPORT virtual bool findVariableType(Type *&type, std::string name)= 0;

      SYMTABEXPORT virtual ~LookupInterface();
};


}//namespace SymtabAPI
}//namespace Dyninst

#endif /* !defined(_Symbol_h_) */
