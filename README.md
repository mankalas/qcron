# qcron

qcron is a cron-like scheduler based on Qt. Using a pattern following
cron syntax, a QCron object will emit signals when the system time
(given by QDateTime) enters a valid time table or when it leaves such
time table.

# Example

```c++
/* From monday to friday, every minutes from 8:00 to 11:59 and from
14:00 to 17:59. */
QCron cron("* 8-11,14-17 * * 1-5 *");
connect(&cron, SIGNAL(activated()),   this, SLOT(onActivation()));
connect(&cron, SIGNAL(deactivated()), this, SLOT(onDeactivation()));
```

The 'activated' signal will be emitted every weekday at 8.00 and
14.00, and the 'deactivated' signal will be emitted every weekday at
12.00 and 18.00.
