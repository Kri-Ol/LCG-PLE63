        program main

        use lcg

        implicit none

        real(8)    :: r
        integer(8) :: s
        integer(4) :: k

        write(*,*) 'TEST'

        call initialize_prng()

        s = seed()
        write(*,*) 'I = ', s

        k = 0
        do while (k < 99)
            r = prn()
            s = seed()
            write(*,*) 's = ', k, s
            k = k + 1
        end do

        call prn_skip(-77_8)
        s = seed()
        r = prn()
        write(*,*) 'S = ', s, r
        end program main
