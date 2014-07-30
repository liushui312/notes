#!/bin/bash

function create_jelly() {
    jelly="../../../email-templates/myscript.jelly"
    rm $jelly
    cp ../../../email-templates/html.jelly $jelly

    insert_string=$(echo -e '<!-- TEST RESULT -->\n<TABLE width="100%" cellpadding="0" cellspacing="0"><TR><TD class="bg1"><B>TEST RESULT</B></TD></TR></TABLE>\n<TABLE border="1" width="60%">\n<TR><TH>Testcase</TH><TH>Result</TH><TH>Exit value</TH></TR>\n')
    echo "${insert_string}"  >> $jelly

    cat ${TEST_RESULT} | grep 'FAIL' > tm1.txt
    while read line
    do
        testcase_fail=$(echo $line | awk '{print $1}')
        exit_value_fail=$(echo $line | awk '{print $3}')
        
        insert_string="<TR><TD>"${testcase_fail}"</TD><TD><font color=\"red\">FAIL</font></TD><TD>"${exit_value_fail}"</TD></TR>"
        insert_string="${insert_string}$(echo -e '\n')"
        echo "${insert_string}"  >> $jelly

        let fail_count++
    done < tm1.txt  

    cat ${TEST_RESULT} | grep 'PASS' > tm2.txt
    while read line
    do
        testcase_pass=$(echo $line | awk '{print $1}')
        
        insert_string="<TR><TD>"${testcase_pass}"</TD><TD>PASS</TD><TD>0</TD></TR>"
        insert_string="${insert_string}$(echo -e '\n')"
        echo "${insert_string}"  >> $jelly

        let pass_count++
    done < tm2.txt

    rm tm1.txt tm2.txt

    let 'total_count=pass_count + fail_count'

    insert_string=$(echo -e '<BR/>\n</TABLE>\n<TABLE border="1" width="60%">\n<TR><TH>Total</TH><TH>PASS</TH><TH>FAIL</TH></TR>\n')
    echo "${insert_string}"  >> $jelly

    insert_string="<TR><TD>"${total_count}"</TD><TD>${pass_count}</TD><TD><font color=\"red\">${fail_count}</font></TD></TR>"
    echo "${insert_string}"  >> $jelly

    insert_string=$(echo -e '</TABLE>\n<BR/>\n</BODY>\n</j:jelly>\n')
    echo "${insert_string}"  >> $jelly
}

function exit_handle() {
    echo "stop ltp-ddt by signal!"
    exit 2
}

# begin
trap exit_handle SIGINT SIGQUIT SIGTERM

start_time=`date +%s`
date=`date +%F_%H-%M-%S`
TEST_RESULT="output/${date}/p2_${date}_result.txt"

pass_count=0
fail_count=0
total_count=0

adb shell sh /data/ltp/before_test.sh
adb shell "rm /data/ltp/output /data/ltp/results -rf"
adb shell sh /data/ltp/runltp -o p2_${date}_ltpscreen.txt -l p2_${date}_result.txt -C p2_${date}_failed.txt -p
ret=$?
if [ ! $ret -eq 0 ]; then
    echo "run ltp-ddt failed!"
    fail=1
    exit 1
fi 

mkdir -p ./output/${date}/
adb pull /data/ltp/output/p2_${date}_failed.txt ./output/${date}/
adb pull /data/ltp/results/p2_${date}_result.txt ./output/${date}/
adb pull /data/ltp/output/p2_${date}_ltpscreen.txt ./output/${date}/
adb pull /data/ltp/output/logcat/   ./output/${date}/logcat

adb shell "rm /data/ltp/output /data/ltp/results -rf" 

end_time=`date +%s`
run_time=$(($end_time - $start_time))

h=`expr $run_time / 3600`
m=`expr $run_time / 60 % 60`
s=`expr $run_time % 60`
echo "run_time: ${h} h ${m} m ${s} s"

create_jelly

if [ $fail_count -eq 0 ]; then
    echo "ltp-ddt test succeeful!"
    exit 0
else
    echo "ltp-ddt test failed!"
    exit 2
fi
