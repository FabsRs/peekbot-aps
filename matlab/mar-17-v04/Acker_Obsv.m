function [L] = Acker_Obsv(A, C, p)
% Design of an observer using Ackermann's formula

n = size(A, 1);
Obsv = [C; C*A];

p1plusp2 = - (p(1,1) + p(1,2));
p1timesp2 = p(1,1) * p(1,2);
alpha_e = A^2 + (p1plusp2)*A  + (p1timesp2)*eye(n);

L = (alpha_e)* inv(Obsv)*[0; 1];
end