# WebServ Stability Tests

This document contains stress tests to verify the stability and robustness of the webserv implementation.

## Prerequisites

- Ensure `siege` is installed: `sudo apt-get install siege`
- Have the webserv binary compiled and configuration file ready

## Test 1: Memory Leak Test (5 Minutes)

### Purpose
Verify that the server doesn't have memory leaks under sustained load.

### Steps

**Terminal 1: Run Server**
```bash
make && ./webserv configuration_files/tester.conf
```

#### Terminal 2: Find & Monitor PID
```bash
# First, find the PID
ps aux | grep webserv

# Then, monitor it (replace <PID> with the actual process ID)
top -p <PID>
```

**Terminal 3: Start Siege**
```bash
siege -c50 -t5M -b http://localhost:8080/index.html
```

### ✅ Success Criteria
Watch the **RES** column in Terminal 2. Memory usage may fluctuate, but it **must not grow steadily** throughout the entire 5-minute test period.

---

## Test 2: Hanging Connections Test (1 Minute)

### Purpose
Ensure the server properly closes connections and doesn't leave them hanging.

### Steps

**Terminal 1: Run Server**
```bash
# If not already running:
make && ./webserv configuration_files/tester.conf
```

**Terminal 2: Run Siege & Check Connections**
```bash
# Run siege for 1 minute, wait 5 seconds, then check connections
siege -c50 -t1M -b http://localhost:8080/index.html && sleep 5 && netstat -an | grep ":8080"
```

### ✅ Success Criteria
After the test completes, the `netstat` output should **only show one line** with the state `LISTEN`. If you see multiple connections in `CLOSE_WAIT` state, there's a connection handling problem.

---

## Test 3: Long-Term Stability Test (15+ Minutes)

### Purpose
Verify the server can handle prolonged stress without crashing.

### Steps

**Terminal 1: Run Server**
```bash
# If not already running:
make && ./webserv configuration_files/tester.conf
```

**Terminal 2: Start Indefinite Siege**
```bash
siege -c25 -b http://localhost:8080/index.html
```

### ✅ Success Criteria
- Let the test run for **at least 15 minutes**
- Press `Ctrl+C` in Terminal 2 to stop siege
- The server process in Terminal 1 **must still be running**
- If the server process has disappeared, it indicates a crash

---

## Common Issues and Troubleshooting

### Memory Leaks
- **Symptom**: RES memory continuously increases
- **Cause**: Not properly freeing allocated memory
- **Solution**: Review malloc/free pairs, check for leaked file descriptors

### Hanging Connections
- **Symptom**: Multiple CLOSE_WAIT connections remain
- **Cause**: Not properly closing socket connections
- **Solution**: Ensure all accepted connections are properly closed

### Server Crashes
- **Symptom**: Server process terminates during stress test
- **Cause**: Segmentation faults, uncaught exceptions, or resource exhaustion
- **Solution**: Use debugging tools like `gdb` or `valgrind` to identify the root cause

## Additional Monitoring Commands

```bash
# Monitor file descriptors
lsof -p <PID>

# Check memory usage over time
watch -n 1 'ps -p <PID> -o pid,vsz,rss,pcpu,pmem,comm'

# Monitor network connections
watch -n 1 'netstat -an | grep :8080'
```