function dy = flight_vacuum(y, t)

  G = 6.672e-11;
  M = 5.98e24;
  W = 7.27e-5;
  
  dy(1) = y(4);
  dy(2) = y(5);
  dy(3) = y(6);

  dy(4) = -(G*M)/(y(1)^2) + y(1)*(y(5)^2) + ((W + y(6))^2)*y(1)*(cos(y(2))^2);
  dy(5) = (-1/y(1))*( y(1)*cos(y(2))*sin(y(2))*(W+y(6))^2 + 2*y(4)*y(5));
  dy(6) = (-1/(y(1)*cos(y(2))))*(2*(W*(y(4)-y(1)*y(5))+y(4)*y(6))*cos(y(2)) - 
           2*y(1)*y(6)*y(5)*sin(y(2)));

endfunction  
