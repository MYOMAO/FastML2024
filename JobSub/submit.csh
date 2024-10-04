set i=1
set f=10



while ( $i < $f )

             
		condor_submit condor.sh

        @ i = $i + 1


end

