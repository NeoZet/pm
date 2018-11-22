global ORDER;
ORDER=3;
A = [1.80 2.50 4.60; 
     3.10 2.30 -1.20; 
     4.51 -1.80 3.60];
b = [2.20 3.60 -1.70];

function solve = gauss(A, b, order)
    tmp = 0;
    for i=1:order
        for j=1:order
            if i != j && A(i,i) == 0 && A(j,i) != 0 && A(i,j) != 0
                for k=1:order
                    A([j,i],:) = A([i,j],:);
                end
                b([i,j]) = b([j,i]);
                break;
            end
        end 
    end
    for i=1:order
        if A(i,i) == 0
            solve = "e";
            return;
        end
        for j=i+1:order
            tmp = A(j,i) / A(i,i);
            for t=i:order
                A(j,t) = A(j,t) - tmp * A(i,t);
            end
            b(j) -= tmp * b(i);
        end
    end
    solve = zeros(order,1);
    for i=order:-1:1
        sum_known_elems = 0;
        for j=1:order
            sum_known_elems = sum_known_elems + A(i,j) * solve(j);
        end
        solve(i) = (b(i) - sum_known_elems) / A(i,i);
    end
end

function resid_res = resid(A, b, solve)
    resid_res = (A * solve).' - b;
end

solve = gauss(A,b,ORDER);
residial = resid(A, b, solve);

if solve == "e"
    printf("Решений нет!\n");
else
    for i=1:ORDER
        printf("X%d = %f\n", i, solve(i));
    end
    printf("=================================\n");
    for i=1:ORDER
        printf("F%d = %f\n", i, residial(i));
    end
    printf("Residial norm: %f\n", max(abs(residial)));
end
