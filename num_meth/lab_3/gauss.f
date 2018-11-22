      subroutine gauss (n,matr,extension,solve)
      real, dimension(n,n) :: matr
      real, dimension(n,n+1) :: ext_matr
      real, dimension(n) :: ni,extension,solve

      do j=1,n
         ext_matr(:,j)=matr(:,j)
      end do
      ext_matr(:,n+1)=extension
      do j=1,n
         ni(j)=0
      end do
      nj=n+1
      do ij=1,n
         b=0.
         do i=1,n
            if (ni(i)==0) then
               do j=1,n
                  c=abs(ext_matr(i,j))
                  d=abs(b)
                  if (c>d) then
                     b=ext_matr(i,j)
                     k=i
                     l=j
                  end if
               enddo
            end if
         end do
         ni(k)=l
         ext_matr(k,l)=0.
         d=abs(b)
         if (d==0.0) then
            print *,'Решений Нет!'
            stop
         end if
         do i=1,nj
            c=abs(ext_matr(k,i))
            if (c/=0.0) then
               s=ext_matr(k,i)/b
               ext_matr(k,i)=s
               do j=1,n
                  ext_matr(j,i)=ext_matr(j,i)-ext_matr(j,l)*s
               end do
            end if
         end do
         do j=1,n
            ext_matr(j,l)=0.
         end do
      end do
      do j=1,n
         k=ni(j)
         solve(k)=ext_matr(j,n+1)
      end do
      return
      end subroutine gauss 
      
c$$$      function resid(n, matr, solve, extesion)
c$$$      real, dimension(n,n) :: matr
c$$$      real, dimension(n) :: extesion, solve
c$$$      real, dimension(n) :: resid
c$$$      resid = matmul(matr, solve) - extesion
c$$$      resid = maxval(abs(matmul(matr,solve) - extesion))
c$$$      end function resid
      
      program main
      IMPLICIT NONE
      integer :: n
      parameter (n=3)
      real, dimension(n,n) :: matrix
      real, dimension(n) :: extension, solve, resid
      real :: norm 
!matrix(1,:)=(/0., 0., 0./)
      matrix(1,:)=(/1.80, 2.50, 4.60/)
      matrix(2,:)=(/3.10, 2.30, -1.20/)
      matrix(3,:)=(/4.51, -1.80, 3.60/)
      extension =(/2.20,3.60,-1.70/)    

      call gauss(n, matrix, extension, solve)

      resid = matmul(matrix, solve) - extension
      norm = maxval(abs(resid))

      print*,'X1 = ',solve(1)
      print*,'X2 = ',solve(2)
      print*,'X3 = ',solve(3)
      print*,'resid = (', resid, ')'
      print*,'resid norm = ', norm
      end program main
