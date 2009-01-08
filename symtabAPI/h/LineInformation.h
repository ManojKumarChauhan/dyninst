/*
 * Copyright (c) 1996-2004 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * This license is for research uses.  For such uses, there is no
 * charge. We define "research use" to mean you may freely use it
 * inside your organization for whatever purposes you see fit. But you
 * may not re-distribute Paradyn or parts of Paradyn, in any form
 * source or binary (including derivatives), electronic or otherwise,
 * to any other organization or entity without our permission.
 * 
 * (for other uses, please contact us at paradyn@cs.wisc.edu)
 * 
 * All warranties, including without limitation, any warranty of
 * merchantability or fitness for a particular purpose, are hereby
 * excluded.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * Even if advised of the possibility of such damages, under no
 * circumstances shall we (or any other person or entity with
 * proprietary rights in the software licensed hereunder) be liable
 * to you or any third party for direct, indirect, or consequential
 * damages of any character regardless of type of action, including,
 * without limitation, loss of profits, loss of use, loss of good
 * will, or computer failure or malfunction.  You agree to indemnify
 * us (and any other person or entity with proprietary rights in the
 * software licensed hereunder) for any and all liability it may
 * incur to third parties resulting from your use of Paradyn.
 */

#if ! defined( LINE_INFORMATION_H )
#define LINE_INFORMATION_H

#include "symutil.h"
#include "RangeLookup.h"
#include "Serialization.h"
#include "Annotatable.h"

namespace Dyninst{
namespace SymtabAPI{

/* This is clumsy. */

namespace LineInformationImpl {
   class LineNoTuple{
      public:
         SYMTABEXPORT LineNoTuple(const char *file_, unsigned int line_, unsigned int col_ = 0);
         const char *first; // really file
         unsigned int second; // really line
         unsigned int column;
         SYMTABEXPORT bool operator==(const LineNoTuple &cmp) const;
   };

   /* Explicit comparison functors seems slightly less confusing than using
      operator <() via an implicit Less<> template argument to the maps. */
   struct LineNoTupleLess {
      bool operator () ( LineNoTuple lhs, LineNoTuple rhs ) const;
   };
} /* end namespace LineInformationImpl */			

SYMTABEXPORT typedef LineInformationImpl::LineNoTuple LineNoTuple;
class SourceLineInternalTableWrapper;

class LineInformation : public Serializable, 
                        public AnnotatableSparse,
                        private RangeLookup< LineInformationImpl::LineNoTuple, LineInformationImpl::LineNoTupleLess > 
{
   public:
      SYMTABEXPORT void serialize(SerializerBase *, const char * = "LineInformation");
      typedef LineInformationImpl::LineNoTuple LineNoTuple;
      typedef RangeLookup< LineInformationImpl::LineNoTuple, LineInformationImpl::LineNoTupleLess >::const_iterator const_iterator;
      typedef RangeLookup< LineInformationImpl::LineNoTuple, LineInformationImpl::LineNoTupleLess >::AddressRange AddressRange;

      SYMTABEXPORT LineInformation();

      /* You MAY freely deallocate the lineSource strings you pass in. */
      SYMTABEXPORT bool addLine( const char * lineSource, 
            unsigned int lineNo, 
            unsigned int lineOffset, 
            Offset lowInclusiveAddr, 
            Offset highExclusiveAddr );
      SYMTABEXPORT void addLineInfo(LineInformation *lineInfo);	      
      SYMTABEXPORT bool addAddressRange( Offset lowInclusiveAddr, 
            Offset highExclusiveAddr, 
            const char * lineSource, 
            unsigned int lineNo, 
            unsigned int lineOffset = 0 );

      /* You MUST NOT deallocate the strings returned. */
      SYMTABEXPORT bool getSourceLines( Offset addressInRange, std::vector< LineNoTuple > & lines );
      SYMTABEXPORT bool getAddressRanges( const char * lineSource, unsigned int LineNo, std::vector< AddressRange > & ranges );

      SYMTABEXPORT const_iterator begin() const;
      SYMTABEXPORT const_iterator end() const;
      SYMTABEXPORT unsigned getSize() const;

      SYMTABEXPORT ~LineInformation();

      // double secret private:
      SourceLineInternalTableWrapper *getSourceLineNamesW();
      SourceLineInternalTableWrapper *sourceLineNamesPtr;

   protected:
      /* We maintain internal copies of all the source file names.  Because
         both directions of the mapping include pointers to these names,
         maintain a separate list of them, and only ever deallocate those
         (in the destructor).  Note that it speeds and simplifies things
         to have the string pointers be the same. */

      unsigned size_;
}; /* end class LineInformation */

}//namespace SymtabAPI
}//namespace Dyninst

#endif /* ! LINE_INFORMATION_H */
