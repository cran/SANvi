#include <RcppArmadillo.h>
#include "D_CAVI_UPDATES.h"
#include "E_ELBO_COMPONENTS.h"

// [[Rcpp::export]]
Rcpp::List main_vb_cam_cpp(arma::field<arma::colvec> Y_grouped,
                           int const L,
                           int const K,
                           int const J,
                           arma::field<arma::mat> XI_ijl,
                           arma::mat RHO_jk,
                           arma::colvec Nj,
                           double m0,
                           double k0,
                           double a0,
                           double b0,
                           arma::colvec ml,
                           arma::colvec kl,
                           arma::colvec al,
                           arma::colvec bl,
                           double const a_tilde,
                           double const b_tilde,
                           double const a_bar,
                           double const b_bar,
                           double epsilon,
                           int maxSIM,
                           bool verbose = 0){

  // random init
  arma::mat ElnOM_lk(L,K);
  arma::mat a_bar_Ulk(L,K);
  arma::mat b_bar_Ulk(L,K);

  arma::colvec  ELBO_val(maxSIM);

  arma::mat var_par_theta(L,4);
  arma::mat var_par_v(K,3);

  int Q = maxSIM;
  arma::colvec a_vk(K);
  arma::colvec b_vk(K);
  arma::colvec E_ln_PIk(K);

  //arma::mat XXX(maxSIM,6);

  for(int ii = 0; ii<maxSIM; ii++){
    R_CheckUserInterrupt(); 
    // omega
    //Rcout << "*\n";
    arma::cube UU = Update_Ulk_cpp(XI_ijl,
                                   RHO_jk,
                                   a_bar,
                                   b_bar,
                                   L,
                                   J,
                                   K);

    a_bar_Ulk = UU.slice(0);
    b_bar_Ulk = UU.slice(1);
    ElnOM_lk  = UU.slice(2);
    //Rcout << "2*\n";

    double ELBO_U     = elbo_p_U(a_bar_Ulk,
                                 b_bar_Ulk,
                                 a_bar,
                                 b_bar,
                                 L,
                                 K) -
                        elbo_q_U(a_bar_Ulk,
                                 b_bar_Ulk,
                                 L,
                                 K);

    // M
    XI_ijl = Update_XIijl_cpp_CAM(Y_grouped,
                              RHO_jk,
                              ElnOM_lk,
                              Nj,
                              ml,
                              kl,
                              al,
                              bl,
                              L,
                              J,
                              K);
    //Rcout << "3.5*\n";

    double ELBO_M = elbo_p_M_CAM(XI_ijl,
                             RHO_jk,
                             ElnOM_lk,
                             L, K, J) -
                               elbo_q_M(XI_ijl, J);
    //Rcout << "*\n";

    // THETA
    var_par_theta  = Update_THETAl_cpp(Y_grouped,
                                       XI_ijl,
                                       m0,
                                       k0,
                                       a0,
                                       b0,
                                       L,
                                       J);
    ml = var_par_theta.col(0);
    kl = var_par_theta.col(1);
    al = var_par_theta.col(2);
    bl = var_par_theta.col(3);

    double ELBO_THETA  = elbo_p_THETA(m0, k0, a0, b0,
                                      ml, kl, al, bl)-
                          elbo_q_THETA(ml, kl,al,bl);
    //Rcout << "*\n";

    // V
    var_par_v = Update_Vk_cpp(K,
                              a_tilde,
                              b_tilde,
                              RHO_jk);

    a_vk     = var_par_v.col(0);
    b_vk     = var_par_v.col(1);
    E_ln_PIk = var_par_v.col(2);

    double ELBO_V = elbo_p_v(a_vk, b_vk,
                             a_tilde, b_tilde, K) -
                               elbo_q_v(a_vk, b_vk, K);
    //Rcout << "*\n";

    // S
    RHO_jk = Update_RHOjk_cpp_CAM(XI_ijl,
                              E_ln_PIk,
                              ElnOM_lk,
                              L,
                              J,
                              K);
    double ELBO_S = elbo_p_S(RHO_jk,E_ln_PIk) - elbo_q_S(RHO_jk);

    double  Elbo_pLIK = elbo_p_Y(Y_grouped,
                                 XI_ijl,
                                 ml,kl,al,bl,
                                 L,J);

    ELBO_val[ii] =  Elbo_pLIK +
      ELBO_S + ELBO_M + ELBO_V + ELBO_U + ELBO_THETA;


  //  XXX(ii,0) = Elbo_pLIK;
  //    XXX(ii,1) = ELBO_S;
  //  XXX(ii,2) = ELBO_M;
  //  XXX(ii,3) = ELBO_V;
  //  XXX(ii,4) = ELBO_U;
  //  XXX(ii,5) = ELBO_THETA;


    if(ii>2) {
      if(verbose){
        Rcpp::Rcout << "Iteration #" << ii+2 << " - Elbo increment: " << (ELBO_val[ii]-ELBO_val[ii-1]) << "\n";
      }
      if(fabs(ELBO_val[ii]-ELBO_val[ii-1]) < epsilon ) {
        if(verbose){
          Rcpp::Rcout << "Final Elbo value: " << (ELBO_val[ii]) << "\n";
        }
        Q = ii;
        break;
      }
    }

  }
  //Rcpp::Rcout << "Convergence reached in " << Q << " iterations";

//  if(Q ==maxSIM){
//    Q = maxSIM-1;
//  }

  arma::colvec ELBO_v2 = ELBO_val.rows(1,Q);
  //arma::mat YYY = XXX.rows(1,Q);

  Rcpp::List results = Rcpp::List::create(
    Rcpp::_["theta_l"]  = var_par_theta,
    Rcpp::_["Elbo_val"] = ELBO_v2,
    Rcpp::_["XI"] = XI_ijl,
    Rcpp::_["RHO"] = RHO_jk,
    // Rcpp::_["ElnPI_k"]  = E_ln_PIk,
    Rcpp::_["a_tilde_k"] = a_vk,
    Rcpp::_["b_tilde_k"] = b_vk,
    Rcpp::_["a_bar_lk"]  = a_bar_Ulk,
    Rcpp::_["b_bar_lk"]  = b_bar_Ulk
    //Rcpp::_["components"]  = YYY
  );
  return(results);
}

