#!/bin/sh

TEST_TIME=4
DELAY_TIME=2

echo Testing LEDD_WASH_SIGNAL_NO_IP
echo -ne '\x00' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME

echo Testing LEDD_WASH_SIGNAL_HAVE_IP
echo -ne '\x01' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME

echo Testing LEDD_WASH_SIGNAL_FIRMWARE_DOWNLOADING
echo -ne '\x00' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME

echo Testing LEDD_WASH_SET_UP_PROCESSING
echo -ne '\x01' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME

echo Testing LEDD_WASH_READY_TO_SETUP
echo -ne '\x02' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME

echo Testing LEDD_WASH_ERROR
echo -ne '\x03' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME

echo Testing LEDD_WASH_HUB_STATUS_WORKING
echo -ne '\x04' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME

echo Testing LEDD_WASH_HUB_STATUS_PARTIALLY_WORKING
echo -ne '\x05' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME

echo Testing LEDD_WASH_HUB_STATUS_NOT_WORKING
echo -ne '\x06' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME

echo Testing LEDD_WASH_SEARCHING_SENSOR
echo -ne '\x07' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME

echo Testing LEDD_WASH_ZIGBEE_DEVICE_CALL_FEEDBACK
echo -ne '\x08' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME

echo Testing LEDD_WASH_FIRMWARE_UPDATE
echo -ne '\x09' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME

echo Testing LEDD_WASH_FACTORY_RESET
echo -ne '\x0c' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME

echo Testing LEDD_WASH_FACTORY_RESET_DONE
echo -ne '\x0d' > ${HUB_TMP_PATH}/ledstate
sleep $TEST_TIME

echo done
