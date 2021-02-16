#!/bin/sh

TEST_TIME=4
DELAY_TIME=2

echo Testing LEDD_SIGNAL_DEVCONN_CONNECTED
echo -ne '\x03' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME

echo Testing LEDD_SIGNAL_ZWAVE_EXCLUDING
echo -ne '\x05' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME
echo -ne '\x04' > ${HUB_TMP_PATH}/ledstate
sleep $DELAY_TIME

echo Testing LEDD_SIGNAL_ZWAVE_REPLACING
echo -ne '\x06' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME
echo -ne '\x04' > ${HUB_TMP_PATH}/ledstate
sleep $DELAY_TIME

echo Testing LEDD_SIGNAL_ZWAVE_INCLUDING
echo -ne '\x07' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME
echo -ne '\x04' > ${HUB_TMP_PATH}/ledstate
sleep $DELAY_TIME

echo Testing LEDD_SIGNAL_DOWNLOADING_UPDATE
echo -ne '\x08' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME
echo -ne '\x09' > ${HUB_TMP_PATH}/ledstate
sleep $DELAY_TIME

echo Testing LEDD_SIGNAL_APPLYING_UPDATE
echo -ne '\x0a' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME
echo -ne '\x0b' > ${HUB_TMP_PATH}/ledstate
sleep $DELAY_TIME

echo Testing LEDD_SIGNAL_ZIGBEE_IDENTIFY
echo -ne '\x0d' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME
echo -ne '\x0c' > ${HUB_TMP_PATH}/ledstate
sleep $DELAY_TIME

echo Testing LEDD_SIGNAL_SOFTAP
echo -ne '\x0e' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME
echo -ne '\x0f' > ${HUB_TMP_PATH}/ledstate
sleep $DELAY_TIME
echo -ne '\x02' > ${HUB_TMP_PATH}/ledstate

echo done
