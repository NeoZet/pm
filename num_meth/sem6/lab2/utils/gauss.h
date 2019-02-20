#pragma once

/*
  Solving linear system of equations by Gauss method
*/
int gauss(double **matrix, double *extension, int order, double *solution);
int resid(double **matrix, double *extension, double *solve, int order, double *resid);
int resid_norm(double *resid, int order, double *norm);
