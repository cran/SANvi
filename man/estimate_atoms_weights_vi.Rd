% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/est_atoms_weights.R
\name{estimate_atoms_weights_vi}
\alias{estimate_atoms_weights_vi}
\alias{plot.vi_atoms_weights}
\alias{print.vi_atoms_weights}
\title{Estimate the Posterior Atoms and Weights of the Discrete Mixing Distributions}
\usage{
estimate_atoms_weights_vi(output)

\method{plot}{vi_atoms_weights}(x, DC_num = NULL, lim = 2, ...)

\method{print}{vi_atoms_weights}(x, thr = 0.01, ...)
}
\arguments{
\item{output}{an object of class \code{SANvb}, which is the output of one of the variational functions \code{\link{variational_CAM}}, 
\code{\link{variational_fiSAN}}, \code{\link{variational_fSAN}}.}

\item{x}{an object of class \code{variational_estimates}, which can be 
obtained from the function \code{\link{estimate_atoms_weights_vi}}.}

\item{DC_num}{an integer or a vector of integers indicating which distributional clusters to plot.}

\item{lim}{optional value for \code{plot} method to adjust the limits of the x-axis. The atoms are plotted on a range
given by \code{min(posterior means)-2, max(posterior means)+2}. Default is set to 2.}

\item{...}{ignored.}

\item{thr}{argument for the \code{print()} method. It should be a small positive number, 
representing a threshold. If the posterior weight of a shared atom is below the threshold, the
atom is not reported.}
}
\value{
an object of class \code{vi_atoms_weights}, which is matrix comprising posterior means, 
variances, and a columns for each estimated DC containing the posterior weights.
}
\description{
This function estimates the posterior atoms and weights characterizing the discrete 
mixing distributions using the variational estimates obtained from one of the model implemented in SANvi.
}
\examples{
# Generate example data
set.seed(1232)
y <- c(rnorm(100),rnorm(100,5))
g <- rep(1:2,rep(100,2))

# Fitting fiSAN via variational inference
est <- variational_fiSAN(y,g,verbose = FALSE)

# Estimate posterior atoms and weights
estimate_atoms_weights_vi(est)
}
\seealso{
\code{\link{variational_CAM}}, \code{\link{variational_fiSAN}}, 
\code{\link{variational_fSAN}}, \code{\link{extract_best}}.
}
