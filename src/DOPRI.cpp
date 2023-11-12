#include "../include/DOPRI.h"

vector<Vector> DOPRI5 (Vector (*f)(Vector), double x0, Vector y0, double b, double epsilon, double sigma) {
      Vector y = y0; 
      vector<Vector> Y = vector<Vector>();
      Y.push_back(y);

      double x = x0; 
      vector<double> X = vector<double>();
      X.push_back(x);

      double h = 3;
      vector<double> H = vector<double>();
      H.push_back(h);

      int safety = 0;
      int max_points = 0;

      while (x < b) {
         Vector k1 = h*(*f)(y);
         Vector k2 = h*(*f)(y + k1/5);
         Vector k3 = h*(*f)(y + k1*0.075 + k2*0.225);
         Vector k4 = h*(*f)(y+44*k1/45-56*k2/15+32*k3/9);
         Vector k5 = h*(*f)(y+19372*k1/6561-25360*k2/2187+64448*k3/6561-212*k4/729);
         Vector k6 = h*(*f)(y+9017*k1/3168-355*k2/33+46732*k3/5247+49*k4/176-5103*k5/18656);
         Vector k7 = h*(*f)(y+35*k1/384+500*k3/1113+125*k4/192-2187*k5/6784+11*k6/84);
         Vector le = 71*k1/57600-71*k3/16695+71*k4/1920-17253*k5/339200+22*k6/525-k7/40;
         double nle = le.norm();

         if(nle<epsilon*h /*|| safety > 10000*/) {
            max_points += 1;
            if (max_points > 250) {
               break;
            }
            y = y + (5179*k1/57600+7571*k3/16695+393*k4/640-92097*k5/339200+187*k6/2100+k7/40);

            Y.push_back(y);                              
            x = x+h;                                                             
            X.push_back(x);
            H.push_back(h);
            h = h*sigma*pow((epsilon*h/nle), (1./5));
            if (x+h > b) {
               h = b-x;
            }
         }
         else {
            h = h/2;                        
         }

        safety = safety + 1;
         if (safety > 10000) {
            break;
         }
      }

      return Y;

    }