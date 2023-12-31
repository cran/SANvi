% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/multistart.R
\name{variational_multistart}
\alias{variational_multistart}
\alias{plot.multistart}
\alias{print.multistart}
\title{Perform variational inference using multiple starting points.}
\usage{
variational_multistart(y, group, runs, cores = 1, 
                       model = c("fiSAN","CAM","fSAN"), 
                       maxL = 30, maxK = 20, 
                       m0 = 0, tau0 = .01, lambda0 = 3, gamma0 = 2, 
                       conc_par = NULL,  conc_hyperpar = c(1,1,1,1), 
                       alpha_bar = 0.05, beta_bar = 0.05, 
                       epsilon = 1e-6, root = 1234, maxSIM = 1e5, 
                       warmstart = TRUE)

\method{plot}{multistart}(x, type = c("elbo", "time"), log_scale_iter = TRUE, ...)

\method{print}{multistart}(x, ...)
}
\arguments{
\item{y}{vector of observations.}

\item{group}{vector of the same length of y indicating the group membership (numeric).}

\item{runs}{the number of multiple runs to launch.}

\item{cores}{the number of cores to dedicate to the multiple runs.}

\item{model}{a string specifying the model to use. It can be \code{"fiSAN"},\code{"CAM"}, or \code{"fSAN"}.}

\item{maxL, maxK}{integers, the upper bounds for the observational and distributional clusters to fit, respectively.}

\item{m0, tau0, lambda0, gamma0}{hyperparameters on \eqn{(\mu, \sigma^2) \sim NIG(m_0, \tau_0, \lambda_0,\gamma_0)}.}

\item{conc_hyperpar, conc_par, alpha_bar, beta_bar}{these values crucially depend on the chosen model. See \code{\link{variational_CAM}}, \code{\link{variational_fiSAN}}, \code{\link{variational_fSAN}} for proper explanations.}

\item{epsilon}{the tolerance that drives the convergence criterion adopted as stopping rule.}

\item{root}{common part of the random seeds used to control the initialization in order to provide reproducibility even in paralleled settings.}

\item{maxSIM}{the maximum number of CAVI iteration to perform.}

\item{warmstart}{logical, if \code{TRUE}, the observational means of the cluster atoms are initialized with a k-means algorithm.}

\item{x}{an object of class \code{multistart}, obtained from the \code{variational_multistart} function.}

\item{type}{a string specifying the type of plot. It can be either \code{"elbo"} or \code{"time"}. The former displays 
the elbo trajectories, highlighting the best run. The latter provides a summary of the computational times.}

\item{log_scale_iter}{logical. If \code{TRUE}, when plotting the elbo trajectories, the x-axes is displayed in log-scale, enhancing the visualization of the results.}

\item{...}{ignored}
}
\value{
A list with all the runs performed. Each element of the list is a fitted variational model of class \code{SANvb}.
}
\description{
\code{variational_multistart} is the main function of the package. 
It is used to estimate via variational inference the three models we present (CAM, fiSAN, fSAN) while adopting multiple random starting points to better explore the variational parameter space.
The run that provides the highest Expected Lower BOund (ELBO) is usually the one considered for inference. Note that the arguments passed to this functions are a union of the arguments 
of the functions \code{variational_CAM}, \code{variational_fiSAN}, and \code{variational_fSAN}.
}
\details{
For the details of the single models, see their specific documentations: \code{\link{variational_CAM}}, \code{\link{variational_fiSAN}}, and \code{\link{variational_fSAN}}.
}
\examples{
\donttest{
# Generate example data
set.seed(123)
y <- c(rnorm(100),rnorm(100,5))
g <- rep(1:2,rep(100,2))

# Estimate multiple models via variational inference
est <- variational_multistart(y, g, runs=5)
}
}
\seealso{
\code{\link{variational_CAM}}, \code{\link{variational_fiSAN}}, \code{\link{variational_fSAN}}, \code{\link{extract_best}}.
}
