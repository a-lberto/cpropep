function rho = density(y)

  P0 = 1.013e5;     # standard stmospheric pressure
  Ma = 0.0289644;   # atmosphere molar mass
  Rg = 8.3145;      # perfect gas constant
  T  = 273.15;      # standard temperature 
  
  G  = 6.67259e-11; # gravitational constant 
  Mt = 5.98e24;     # earth mass
  Rt = 6370000;     # earth radius
  
  rho = (P0*Ma/(Rg*T))*exp( ((Ma*G*Mt)/(Rg*T))*((1/(Rt+y))-(1/Rt)) );

endfunction
