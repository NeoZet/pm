ORDER=3;
A = [5 -3 1; 
     1 -10 3; 
     -2 1 4];
b = [1 2 -8];

function [solve, iter_num] = jacobi_method(matr, extens, order, eps)
    solve = zeros(1, order);
    solve_prev = solve;
    solve_prev(1) = 1;
    iter_num = 0;
    accur = eps + 1;
    while accur > eps
        for i=1:order
            sum = 0;
            for j=1:order
                if i != j
                    sum = sum - matr(i,j) * solve_prev(j);
                end    
            end
            solve(i) = (sum + extens(i)) / matr(i,i);
        end
        accur = max(abs(solve_prev - solve)) / max(abs(solve));
        solve_prev = solve;
        iter_num = iter_num + 1;
    end
end

function [solve, iter_num] = gauss_seidel(matr, extens, order, eps)
    solve = zeros(1, order);
    solve_prev = solve;
    solve_prev(1) = 1;
    iter_num = 0;
    accur = eps + 1;
    while accur > eps
        for i=1:order
            sum = 0;
            for j=1:i-1
                    sum = sum - matr(i,j) * solve(j);
            end
            for j=i+1:order
                    sum = sum - matr(i,j) * solve_prev(j);
            end
            solve(i) = (sum + extens(i)) / matr(i,i);
        end
        accur = max(abs(solve_prev - solve)) / max(abs(solve));
        solve_prev = solve;
        iter_num = iter_num + 1;
    end
end

function resid_res = resid(A, b, solve)
    resid_res = (A * solve.').' - b;
end

[sol, num_iter] = jacobi_method(A,b,ORDER,0.0001);
residial = resid(A, b, sol);
printf("JACOBI\n***********\n");
printf("Iterations: %d\n", num_iter);
for i=1:ORDER
    printf("X%d = %f\n", i, sol(i));
end 
    printf("=================================\n");
for i=1:ORDER
    printf("F%d = %f\n", i, residial(i));
end
printf("Residial norm: %f\n", max(abs(residial)));
printf("----------------------------------------------------\n");
[sol, num_iter] = gauss_seidel(A,b,ORDER,0.0001);
residial = resid(A, b, sol);
printf("GAUSS_SEIDEL\n***********\n");
printf("Iterations: %d\n", num_iter);
for i=1:ORDER
    printf("X%d = %f\n", i, sol(i));
end 
    printf("=================================\n");
for i=1:ORDER
    printf("F%d = %f\n", i, residial(i));
end
printf("Residial norm: %f\n", max(abs(residial)));
