Overview
========

This repository contains an Unreal Engine plugin for logging information to the viewport and/or output log. Logs contain which class and function they originate from, as well as if they are coming from clients or server.


Example Usage
------------

**C++**
```C++
#include "EasyLoggerStatics.h"

void AEasyLoggerDemo::DemoEZLog()
{
	UEasyLoggerStatics::EZLog(this, __FUNCTION__, "This is a Display log.");
	UEasyLoggerStatics::EZLog(this, __FUNCTION__, "This is a Warning log.", EEZLogSeverity::Warning);
	UEasyLoggerStatics::EZLog(this, __FUNCTION__, "This is an Error log.", EEZLogSeverity::Error);
}
```

**Blueprint**
![BP Demo](ReadmeImages/BPDemo.png)

**Output Log**
![Output Log](ReadmeImages/OutputLog.png)

**Viewport Output**  
![Viewport Output](ReadmeImages/ViewportOutput.png)
