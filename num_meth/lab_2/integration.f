      module constants
      implicit none
      real*8, parameter  :: lower_limit=0, upper_limit=4
      real*8             :: start_step=1
      real*8             :: eps_4 = 0.0001, eps_5 = 10**(-5)
      integer, parameter :: MAX_ARR_SIZE = 1024
      end module constants
     
      
      function integrate_function(x) result(res)
      real*8, intent(in) :: x
      real*8             :: res
      res = 1.0/(1 + x**(0.5))
      end function integrate_function
      
      
      subroutine fill_arrays(x_array, y_array, arr_size, step)
      use constants
      real*8, intent(in) :: step
      real*8             :: integrate_function
      real*8             :: x_array(MAX_ARR_SIZE)
      real*8             :: y_array(MAX_ARR_SIZE)
      real*8             :: value
      integer            :: iter, arr_size
      
      iter  = 0
      value = lower_limit
      
      do while (value <= upper_limit .and. iter < MAX_ARR_SIZE)
        x_array(iter) = value
        y_array(iter) = integrate_function(value)
        value = value + step
        iter = iter + 1
      end do
      arr_size = iter
      end subroutine fill_arrays

      function integral_by_trapezoid(array,arr_size,h) result(integral)
      use constants      
      integer, intent(in)   :: arr_size      
      real*8, intent(in)    :: h
      real*8                :: integral
      real*8                :: array(MAX_ARR_SIZE)
      real*8                :: array_sum=0
      do i = 1, arr_size / 2 + 1
        array_sum = array_sum + array(i)
      end do      
      integral = h/2 * (array(0) + 2*array_sum + array(arr_size-1))
      end function integral_by_trapezoid
      
      function integral_calculation(step) result(integral)
      use constants
      real*8    :: integral
      real*8    :: x_list(MAX_ARR_SIZE)
      real*8    :: y_list(MAX_ARR_SIZE)
      real*8    :: h, step
      real*8    :: integral_by_trapezoid
      integer :: list_size
      call fill_arrays(x_list, y_list, list_size, step)
      h = (x_list(list_size-1) - x_list(0)) / list_size
      integral = integral_by_trapezoid(y_list, list_size, h)      
      end function integral_calculation
      
      subroutine check_accuracy(integral, accuracy, step)
      use constants
      real*8 :: step
      real*8 :: integral, accuracy      
      real*8 :: integral_tmp, accuracy_tmp
      real*8 :: integral_calculation
      real*8 :: step_t
      step_t=step
      integral_tmp = integral_calculation(step)
      accuracy_tmp = dabs(integral_calculation(step/2)-integral_tmp)/3
      step_t = step
      do while (eps_4 < accuracy_tmp)
        print *, integral_tmp, accuracy_tmp, eps_4
        integral_tmp = integral_calculation(step_t)
        accuracy_tmp=dabs(integral_calculation(step_t/2)-integral_tmp)/3
        step_t = step_t / 10
      end do
      integral = integral_tmp
      accuracy = accuracy_tmp
      end subroutine check_accuracy

      program main
      use constants
      real*8 :: integrate_function
      real*8 :: integral
      real*8 :: accuracy
      real*8 :: stp=1
      !call fill_arrays(x_list, y_list, list_size, start_step/2)
      call check_accuracy(integral, accuracy, stp)
      print*, "integral",integral,"accuracy",accuracy
c$$$      do while (iter < list_size)
c$$$        print*, "x",x_list(iter),"y",y_list(iter)
c$$$        iter = iter + 1
c$$$      enddo
c$$$      print*, "var",MAX_ARR_SIZE
      end program main
 
