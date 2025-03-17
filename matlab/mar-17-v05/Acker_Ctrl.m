function [K] = Acker_Ctrl(A, B, p)
% Design of an observer using Ackermann's formula
n = size(A, 1);
Ctrb = [B A*B];

p1plusp2 = - (p(1,1) + p(1,2));
p1timesp2 = p(1,1) * p(1,2);
alpha_c = A^2 + (p1plusp2)*A + (p1timesp2)*eye(n);

K = [zeros(1, n-1), 1] *(Ctrb \ alpha_c);
end