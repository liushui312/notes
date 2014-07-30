
AlarmManager：  在特定的时刻为我们广播一个指定的Intent。

android提供了四种类型的闹钟：
    ELAPSED_REALTIME          在指定的延时过后，发送广播，但不唤醒设备
    ELAPSED_REALTIME_WAKEUP   在指定的延时过后，发送广播，并唤醒设备 
    RTC                       在指定的时刻，发送广播，但不唤醒设备
    RTC_WAKEUP                在指定的时刻，发送广播，并唤醒设备


设置闹钟:
final AlarmManager alarmMgr = (AlarmManager) mContext
                    .getSystemService(Context.ALARM_SERVICE);

final PendingIntent pendIntent = PendingIntent.getBroadcast(
                    mContext, 0, new Intent(mContext, AlarmReceiver.class),
                                        PendingIntent.FLAG_UPDATE_CURRENT);




重要方法：
    对象.setExact(AlarmManager.RTC_WAKEUP, timeInMillis, pendingIntent);  4.4
    set(int type, long triggerAtMillis, PendingIntent operation);
