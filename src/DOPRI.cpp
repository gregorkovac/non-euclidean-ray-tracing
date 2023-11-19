#include "../include/DOPRI.h"

Vector4 F_spherical(Vector4 x)
{
   return Vector4(x.y, cosl(x.x) * sinl(x.x) * x.w * x.w, x.w, -2 * 1 / tanl(x.x) * x.y * x.w);
}

vector<Vector4> DOPRI5(Vector4 (*f)(Vector4), double x0, Vector4 y0, double b, double epsilon, double sigma)
{
   Vector4 y = y0;
   vector<Vector4> Y = vector<Vector4>();
   Y.push_back(y);

   double x = x0;
   vector<double> X = vector<double>();
   X.push_back(x);

   double h = 3;
   vector<double> H = vector<double>();
   H.push_back(h);

   int safety = 0;
   int max_points = 0;

   while (x < b)
   {
      Vector4 k1 = h * (*f)(y);
      Vector4 k2 = h * (*f)(y + k1 / 5);
      Vector4 k3 = h * (*f)(y + k1 * 0.075 + k2 * 0.225);
      Vector4 k4 = h * (*f)(y + 44 * k1 / 45 - 56 * k2 / 15 + 32 * k3 / 9);
      Vector4 k5 = h * (*f)(y + 19372 * k1 / 6561 - 25360 * k2 / 2187 + 64448 * k3 / 6561 - 212 * k4 / 729);
      Vector4 k6 = h * (*f)(y + 9017 * k1 / 3168 - 355 * k2 / 33 + 46732 * k3 / 5247 + 49 * k4 / 176 - 5103 * k5 / 18656);
      Vector4 k7 = h * (*f)(y + 35 * k1 / 384 + 500 * k3 / 1113 + 125 * k4 / 192 - 2187 * k5 / 6784 + 11 * k6 / 84);
      Vector4 le = 71 * k1 / 57600 - 71 * k3 / 16695 + 71 * k4 / 1920 - 17253 * k5 / 339200 + 22 * k6 / 525 - k7 / 40;
      double nle = le.norm();

      if (nle < epsilon * h /*|| safety > 10000*/)
      {
         max_points += 1;
         if (max_points > 250)
         {
            break;
         }
         y = y + (5179 * k1 / 57600 + 7571 * k3 / 16695 + 393 * k4 / 640 - 92097 * k5 / 339200 + 187 * k6 / 2100 + k7 / 40);

         Y.push_back(y);
         x = x + h;
         X.push_back(x);
         H.push_back(h);
         h = h * sigma * pow((epsilon * h / nle), (1. / 5));
         if (x + h > b)
         {
            h = b - x;
         }
      }
      else
      {
         h = h / 2;
      }

      safety = safety + 1;
      if (safety > 10000)
      {
         break;
      }
   }

   return Y;
}

Vector4 DOPRI5_one_step(Vector4 (*f)(Vector4), double x0, Vector4 y0, double* b, double epsilon, double sigma)
{
   Vector4 y = y0;

   double x = x0;

   double h = *b;

   int safety = 0;

   Vector4 k1 = new Vector4(0,0,0,0);
   Vector4 k2 = new Vector4(0,0,0,0);
   Vector4 k3 = new Vector4(0,0,0,0);
   Vector4 k4 = new Vector4(0,0,0,0);
   Vector4 k5 = new Vector4(0,0,0,0);
   Vector4 k6 = new Vector4(0,0,0,0);
   Vector4 k7 = new Vector4(0,0,0,0);
   Vector4 le = new Vector4(0,0,0,0);
   double nle;

   while (safety < 10)
   {
      k1 = h * (*f)(y);
      k2 = h * (*f)(y + k1 / 5);
      k3 = h * (*f)(y + k1 * 0.075 + k2 * 0.225);
      k4 = h * (*f)(y + 44 * k1 / 45 - 56 * k2 / 15 + 32 * k3 / 9);
      k5 = h * (*f)(y + 19372 * k1 / 6561 - 25360 * k2 / 2187 + 64448 * k3 / 6561 - 212 * k4 / 729);
      k6 = h * (*f)(y + 9017 * k1 / 3168 - 355 * k2 / 33 + 46732 * k3 / 5247 + 49 * k4 / 176 - 5103 * k5 / 18656);
      k7 = h * (*f)(y + 35 * k1 / 384 + 500 * k3 / 1113 + 125 * k4 / 192 - 2187 * k5 / 6784 + 11 * k6 / 84);
      le = 71 * k1 / 57600 - 71 * k3 / 16695 + 71 * k4 / 1920 - 17253 * k5 / 339200 + 22 * k6 / 525 - k7 / 40;
      nle = le.norm();


      if (nle < epsilon * h /*|| safety > 10000*/)
      {
         break;
      }
      else
      {
         h = h / 2;
      }

      safety = safety + 1;
   }

   y = y + (5179 * k1 / 57600 + 7571 * k3 / 16695 + 393 * k4 / 640 - 92097 * k5 / 339200 + 187 * k6 / 2100 + k7 / 40);

   //*b = h * sigma * pow((epsilon * h / nle), (1. / 5));

   return y;
}