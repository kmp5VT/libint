
#include <integral.h>

#ifndef _libint2_src_bin_libint_tig121111_h_
#define _libint2_src_bin_libint_tig121111_h_

using namespace std;

namespace libint2 {
  
#if 0
  /** TiG12_11_11_base is the base for all 2-body integral over the Ti_G12
      operator with one basis function for each particle in bra and ket
    */
  template <class BFSet>
    struct TiG12_11_11_base {
      virtual bool instance_of_TiG12_11_11() =0;
    };

  /**
     Most basic type -- TiG12_11_11 --
     has one bfs for each particle in bra and ket.
     Note that GenIntegralSet is initialized with an abstract type libint2::BFSet,
     from which BFS derives.
  */
  template <class BFS, int I> class TiG12_11_11 :
    public GenIntegralSet< Ti_G12<I>, IncableBFSet, typename DefaultTwoPBraket<BFS>::Result, typename DefaultTwoPBraket<BFS>::Result, EmptySet >,
    public TiG12_11_11_base<BFS>
    {
    public:
      typedef BFS BasisFunctionType;
      typedef Ti_G12<I> OperType;
      typedef typename DefaultTwoPBraket<BFS>::Result BraType;
      typedef typename DefaultTwoPBraket<BFS>::Result KetType;
      typedef EmptySet AuxIndexType;
      typedef TiG12_11_11 this_type;

      /// TiG12_11_11 is a set of these subobjects
      typedef TiG12_11_11<typename BFS::iter_type,I> iter_type;
      /// This is the immediate parent
      typedef GenIntegralSet< OperType, IncableBFSet, BraType, KetType, AuxIndexType > parent_type;
      /// This class provides comparison operations on pointers
      typedef PtrEquiv<this_type> PtrComp;

      typedef typename parent_type::key_type key_type;
      /// This the type of the object that manages GenIntegralSet's as Singletons
      typedef SingletonStack<TiG12_11_11,key_type> SingletonManagerType;
      
      /* This "constructor" takes basis function sets, in Mulliken ordering.
         Returns a pointer to a unique instance, a la Singleton
      */
      static const SafePtr<TiG12_11_11> Instance(const BFS& bra0, const BFS& ket0, const BFS& bra1, const BFS& ket1);
      /// Returns a pointer to a unique instance, a la Singleton
      static const SafePtr<TiG12_11_11> Instance(const BraType& bra, const KetType& ket, const AuxIndexType& aux);
      ~TiG12_11_11();
      
      /// Comparison operator
      bool operator==(const this_type&) const;
#if OVERLOAD_GENINTEGRALSET_LABEL
      /// Specialization of GenIntegralSet::label()
      const std::string& label() const;
#endif

    private:
      // This constructor is also private and not implemented since all Integral's are Singletons. Use Instance instead.
      TiG12_11_11(const BraType& bra, const KetType& ket, const AuxIndexType& aux);

      // This is used to manage GenIntegralSet objects as singletons
      static SingletonManagerType singl_manager_;

      /// Implements DGVertex::this_precomputed()
      bool this_precomputed() const;
      /// Implements TiG12_11_11_base::instance_of_TiG12_11_11()
      bool instance_of_TiG12_11_11() { return true; }
#if OVERLOAD_GENINTEGRALSET_LABEL
      mutable std::string label_;
#endif
    };

#if USE_INT_KEY_TO_HASH
  template <class BFS, int I>
    typename TiG12_11_11<BFS,I>::SingletonManagerType
    TiG12_11_11<BFS,I>::singl_manager_(&TiG12_11_11<BFS,I>::key);
#else
  // I use label() to hash TiG12_11_11. Therefore labels must be unique!
  template <class BFS, int I>
    typename TiG12_11_11<BFS,I>::SingletonManagerType
    TiG12_11_11<BFS,I>::singl_manager_(&TiG12_11_11<BFS,I>::label);
#endif
  
  template <class BFS, int I>
    TiG12_11_11<BFS,I>::TiG12_11_11(const BraType& bra, const KetType& ket,  const AuxIndexType& aux) :
    parent_type(Ti_G12<I>(),bra, ket, aux)
    {
      if (bra.num_members(0) != 1)
        throw std::runtime_error("TiG12_11_11<BFS,I>::TiG12_11_11(bra,ket) -- number of BFSs in bra for particle 0 must be 1");
      if (bra.num_members(1) != 1)
        throw std::runtime_error("TiG12_11_11<BFS,I>::TiG12_11_11(bra,ket) -- number of BFSs in bra for particle 1 must be 1");
      if (ket.num_members(0) != 1)
        throw std::runtime_error("TiG12_11_11<BFS,I>::TiG12_11_11(bra,ket) -- number of BFSs in ket for particle 0 must be 1");
      if (ket.num_members(1) != 1)
        throw std::runtime_error("TiG12_11_11<BFS,I>::TiG12_11_11(bra,ket) -- number of BFSs in ket for particle 1 must be 1");
#if DEBUG
      std::cout << "TiG12_11_11: constructed " << this->label() << std::endl;
#endif
    }

  template <class BFS, int I>
    TiG12_11_11<BFS,I>::~TiG12_11_11()
    {
#if DEBUG
      std::cout << "TiG12_11_11: destructed " << this->label() << std::endl;
#endif
    }

  template <class BFS, int I>
    const SafePtr< TiG12_11_11<BFS,I> >
    TiG12_11_11<BFS,I>::Instance(const BraType& bra, const KetType& ket, const AuxIndexType& aux)
    {
      typedef typename SingletonManagerType::value_type map_value_type;
      key_type key = compute_key(OperType(),bra,ket,aux);
      const map_value_type& val = singl_manager_.find(key);
      if (!val.second) {
	SafePtr<TiG12_11_11> this_int(new TiG12_11_11<BFS,I>(bra,ket,aux));
	// Use singl_manager_ to make sure this is a new object of this type
	const typename SingletonManagerType::value_type& val = singl_manager_.find(this_int);
	val.second->instid_ = val.first;
	return val.second;
      }
      return val.second;
    }

  template <class BFS, int I>
    const SafePtr< TiG12_11_11<BFS,I> >
    TiG12_11_11<BFS,I>::Instance(const BFS& bra0, const BFS& ket0, const BFS& bra1, const BFS& ket1)
    {
#if USE_BRAKET_H
      typedef BFS BFSRef;
      BFSRef bra0_ref(bra0);
      BFSRef bra1_ref(bra1);
      BFSRef ket0_ref(ket0);
      BFSRef ket1_ref(ket1);
#else
      typedef SafePtr<BFS> BFSRef;
      BFSRef bra0_ref(new BFS(bra0));
      BFSRef bra1_ref(new BFS(bra1));
      BFSRef ket0_ref(new BFS(ket0));
      BFSRef ket1_ref(new BFS(ket1));
#endif
      vector<BFSRef> vbra0;  vbra0.push_back(bra0_ref);
      vector<BFSRef> vbra1;  vbra1.push_back(bra1_ref);
      vector<BFSRef> vket0;  vket0.push_back(ket0_ref);
      vector<BFSRef> vket1;  vket1.push_back(ket1_ref);
      vector< vector<BFSRef> > vvbra;  vvbra.push_back(vbra0);  vvbra.push_back(vbra1);
      vector< vector<BFSRef> > vvket;  vvket.push_back(vket0);  vvket.push_back(vket1);
      BraType bra(vvbra);
      KetType ket(vvket);
      AuxIndexType aux(vector<int>(0));
      return Instance(bra,ket,aux);
    }
  
  template <class BFS, int I>
    bool
    TiG12_11_11<BFS,I>::operator==(const TiG12_11_11<BFS,I>& a) const
    {
      return parent_type::PtrComp::equiv(static_cast<const parent_type*>(this),a);
    }

#if OVERLOAD_GENINTEGRALSET_LABEL
  template <class BFS, int I>
    const std::string&
    TiG12_11_11<BFS,I>::label() const
    {
      if (label_.empty()) {
	ostringstream os;
	os << "(" << parent_type::bra_.member(0,0)->label() << " "
	   << parent_type::ket_.member(0,0)->label()
	   << " | [T_" << I+1 << ", G12] | "
	   << parent_type::bra_.member(1,0)->label() << " "
	   << parent_type::ket_.member(1,0)->label() << ")";
	label_ = os.str();
      }
      return label_;
    };
#endif
  
  template <class BFS, int I>
    bool
    TiG12_11_11<BFS,I>::this_precomputed() const
    {
      // These integrals are never precomputed
      return false;
    }
  
  /** This is a collection of utility functions to extract
      compile-time information about instances of TiG12_11_11 at runtime.
    */
  namespace TiG12_11_11_Util {

    template <class BFS>
      int i(const SafePtr< TiG12_11_11_base<BFS> >& ints) {
        typedef TiG12_11_11_base<BFS> base_type;
        // try k=0
        {
          typedef TiG12_11_11<BFS,0> inst_type;
          SafePtr<inst_type> iptr = dynamic_pointer_cast<inst_type,base_type>(ints);
          if (iptr != 0)
            return 0;
        }
        // try k=1
        {
          typedef TiG12_11_11<BFS,1> inst_type;
          SafePtr<inst_type> iptr = dynamic_pointer_cast<inst_type,base_type>(ints);
          if (iptr != 0)
            return 1;
        }
      };

  };
#endif
  
};

#endif

