function dy = flight(y, t)

  G = 6.672e-11;
  M = 5.98e24;
  W = 7.27e-5;

  m = 100; # kg
  Cd = 0.75;
  A = 0.1;
  rho = density(y(1)-6370000);
  Cr = 0.5*Cd*rho*A;

  v = sqrt( y(4)^2 + (y(1)*y(6)*cos(y(2)))^2 + (y(1)*y(5))^2);

  dy(1) = y(4);
  dy(2) = y(5);
  dy(3) = y(6);

  dy(4) = -(G*M)/(y(1)^2) - Cr*v*y(4)/m + y(1)*(y(5)^2) + ((W + y(6))^2)*y(1)*(cos(y(2))^2);
  
  dy(5) = (-1/y(1))*( Cr*v*y(1)*y(5)/m + y(1)*cos(y(2))*sin(y(2))*(W+y(6))^2 +
                      2*y(4)*y(5));
  dy(6) = (-1/(y(1)*cos(y(2)))) * (Cr*v*y(1)*y(6)*cos(y(2))/m +
            2*(W*(y(4)-y(1)*y(5))+y(4)*y(6))*cos(y(2)) -
            2*y(1)*y(6)*y(5)*sin(y(2)));

endfunction  
