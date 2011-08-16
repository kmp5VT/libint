
#include <iostream>
#include <cmath>

#include <rr.h>
#include <iter.h>
#include <policy_spec.h>
#include <global_macros.h>

#include <libint2.h>
#include <libint/util.h>
#include <test_eri/eri.h>
#include <test_eri/prep_libint2.h>
// generated by test_eri.pl
#include <test_eri_conf.h>

using namespace std;
using namespace libint2;

typedef unsigned int uint;

int main(int argc, char** argv)
{
  const uint veclen = LIBINT2_MAX_VECLEN;
#if LIBINT_CONTRACTED_INTS
  const uint contrdepth = 3;
#else
  const uint contrdepth = 1;
#endif
  const uint contrdepth4 = contrdepth * contrdepth * contrdepth * contrdepth;
  RandomShellQuartetSet rsqset(am, veclen, contrdepth);

  const unsigned int deriv_order = 0;
  DerivIndexIterator<4> diter(deriv_order);
  const unsigned int nderiv = diter.range_rank();

  CGShell sh0(am[0]);
  CGShell sh1(am[1]);
  CGShell sh2(am[2]);
  CGShell sh3(am[3]);

  const double* A = &(rsqset.R[0][0]);
  const double* B = &(rsqset.R[1][0]);
  const double* C = &(rsqset.R[2][0]);
  const double* D = &(rsqset.R[3][0]);

  typedef SubIteratorBase<CGShell> iter;
  SafePtr<iter> sh0_iter(new iter(sh0));
  SafePtr<iter> sh1_iter(new iter(sh1));
  SafePtr<iter> sh2_iter(new iter(sh2));
  SafePtr<iter> sh3_iter(new iter(sh3));

  std::vector<Libint_eri0_t> erieval(contrdepth4);
  const int max_am = max(max(am[0],am[1]),max(am[2],am[3]));
  LIBINT2_PREFIXED_NAME(libint2_init_eri0)(&erieval[0],max_am,0);
  prep_libint2(erieval,rsqset,0);

  cout << "Testing (" << sh0.label() << sh1.label()
  << "|" << sh2.label() << sh3.label() << ") ";
  if (deriv_order > 0) {
    cout << " deriv order = " << deriv_order;
  }
  cout << endl;

  double scale_target = 1.0;
#if LIBINT_ACCUM_INTS
  // if accumulating integrals, zero out first, then compute twice
  erieval->zero_out_targets = 1;
  scale_target = 0.5;
  COMPUTE_XX_ERI_XX(erieval);
#endif
#if LIBINT_CONTRACTED_INTS
  erieval[0].contrdepth = contrdepth4;
#endif
  COMPUTE_XX_ERI_XX(&erieval[0]);

  bool success = true;
  int ijkl = 0;
  for(sh0_iter->init(); int(*sh0_iter); ++(*sh0_iter)) {
    for(sh1_iter->init(); int(*sh1_iter); ++(*sh1_iter)) {
      for(sh2_iter->init(); int(*sh2_iter); ++(*sh2_iter)) {
        for(sh3_iter->init(); int(*sh3_iter); ++(*sh3_iter), ijkl++) {

#if USE_BRAKET_H
          CGF bf0 = sh0_iter->elem();
          CGF bf1 = sh1_iter->elem();
          CGF bf2 = sh2_iter->elem();
          CGF bf3 = sh3_iter->elem();

          uint l0 = bf0.qn(0);
          uint m0 = bf0.qn(1);
          uint n0 = bf0.qn(2);
          uint l1 = bf1.qn(0);
          uint m1 = bf1.qn(1);
          uint n1 = bf1.qn(2);
          uint l2 = bf2.qn(0);
          uint m2 = bf2.qn(1);
          uint n2 = bf2.qn(2);
          uint l3 = bf3.qn(0);
          uint m3 = bf3.qn(1);
          uint n3 = bf3.qn(2);
#else
          SafePtr<CGF> bf0 = sh0_iter->elem();
          SafePtr<CGF> bf1 = sh1_iter->elem();
          SafePtr<CGF> bf2 = sh2_iter->elem();
          SafePtr<CGF> bf3 = sh3_iter->elem();

          uint l0 = bf0->qn(0);
          uint m0 = bf0->qn(1);
          uint n0 = bf0->qn(2);
          uint l1 = bf1->qn(0);
          uint m1 = bf1->qn(1);
          uint n1 = bf1->qn(2);
          uint l2 = bf2->qn(0);
          uint m2 = bf2->qn(1);
          uint n2 = bf2->qn(2);
          uint l3 = bf3->qn(0);
          uint m3 = bf3->qn(1);
          uint n3 = bf3->qn(2);
#endif

          for(uint v=0; v<veclen; v++) {

            std::vector<double> ref_eri(nderiv, 0.0);

            uint p0123 = 0;
            for (uint p0 = 0; p0 < contrdepth; p0++) {
              for (uint p1 = 0; p1 < contrdepth; p1++) {
                for (uint p2 = 0; p2 < contrdepth; p2++) {
                  for (uint p3 = 0; p3 < contrdepth; p3++, p0123++) {

                    const double alpha0 = rsqset.exp[0][v][p0];
                    const double alpha1 = rsqset.exp[1][v][p1];
                    const double alpha2 = rsqset.exp[2][v][p2];
                    const double alpha3 = rsqset.exp[3][v][p3];

                    const double c0 = rsqset.coef[0][v][p0];
                    const double c1 = rsqset.coef[1][v][p1];
                    const double c2 = rsqset.coef[2][v][p2];
                    const double c3 = rsqset.coef[3][v][p3];
                    const double c0123 = c0 * c1 * c2 * c3;

                    DerivIndexIterator<4> diter(deriv_order);
                    bool last_deriv = false;
                    unsigned int di = 0;
                    do {
                      ref_eri[di++] += c0123 * eri(diter.values(),
                                                   l0,m0,n0,alpha0,A,
                                                   l1,m1,n1,alpha1,B,
                                                   l2,m2,n2,alpha2,C,
                                                   l3,m3,n3,alpha3,D,
                                                   0);
                      last_deriv = diter.last();
                      if (!last_deriv) diter.next();
                    } while (!last_deriv);

                  }
                }
              }
            }

            for(unsigned int di = 0; di<nderiv; ++di) {

              const double new_eri = scale_target * erieval[0].targets[di][ijkl*veclen+v];

              if ( fabs((ref_eri[di]-new_eri)/new_eri) > 1.0E-12) {
                std::cout << "Elem " << ijkl << " di= " << di << " v=" << v
                    << " : eri.cc = " << ref_eri[di]
                    << " libint = " << new_eri << endl;
                success = false;
              }
            }

          } // end of vector loop
        }
      }
    }
  }

  LIBINT2_PREFIXED_NAME(libint2_cleanup_eri0)(&erieval[0]);

  cout << "test " << (success ? "ok" : "failed") << endl;

  return 0;
}


