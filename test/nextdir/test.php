<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>C++ Server Monitor</title>

<style>
@import url('https://fonts.googleapis.com/css2?family=JetBrains+Mono:wght@400;500;700&family=Space+Grotesk:wght@400;500;600;700&display=swap');

* { box-sizing: border-box; margin: 0; padding: 0; }

:root {
    --bg: #070909;
    --panel: #0c1010;
    --panel2: #101515;
    --border: #202727;
    --text: #e8eeee;
    --muted: #6f7b7b;
    --green: #00ff88;
    --green-dim: #00ff881c;
    --amber: #ffbd59;
    --red: #ff5f5f;
}

body {
    min-height: 100vh;
    background: var(--bg);
    color: var(--text);
    font-family: "Space Grotesk", sans-serif;
    overflow-x: hidden;
}

body::before {
    content: "";
    position: fixed;
    inset: 0;
    pointer-events: none;
    opacity: .45;
    background-image:
        linear-gradient(rgba(255,255,255,.018) 1px, transparent 1px),
        linear-gradient(90deg, rgba(255,255,255,.018) 1px, transparent 1px);
    background-size: 36px 36px;
}

.shell {
    width: min(1400px, 94%);
    margin: auto;
}

header {
    height: 76px;
    display: flex;
    align-items: center;
    justify-content: space-between;
    border-bottom: 1px solid var(--border);
}

.brand {
    font-family: "JetBrains Mono", monospace;
    font-weight: 700;
    letter-spacing: -.5px;
}

.brand span { color: var(--green); }

.status {
    display: flex;
    align-items: center;
    gap: 9px;
    color: var(--muted);
    font: 12px "JetBrains Mono", monospace;
}

.status-dot {
    width: 8px;
    height: 8px;
    border-radius: 50%;
    background: var(--green);
    box-shadow: 0 0 12px var(--green);
}

.topline {
    padding: 28px 0 22px;
    display: flex;
    justify-content: space-between;
    align-items: end;
}

.eyebrow {
    color: var(--green);
    font: 12px "JetBrains Mono", monospace;
    margin-bottom: 8px;
}

h1 {
    font-size: clamp(30px, 5vw, 54px);
    letter-spacing: -2.5px;
}

.subtitle {
    color: var(--muted);
    margin-top: 8px;
    font: 13px "JetBrains Mono", monospace;
}

.clock {
    color: #849090;
    font: 12px "JetBrains Mono", monospace;
}

.metrics {
    display: grid;
    grid-template-columns: repeat(4, 1fr);
    gap: 12px;
    margin-bottom: 12px;
}

.metric, .panel {
    background: linear-gradient(145deg, var(--panel), #090d0d);
    border: 1px solid var(--border);
}

.metric {
    padding: 19px;
}

.metric-label {
    color: var(--muted);
    font: 11px "JetBrains Mono", monospace;
    text-transform: uppercase;
}

.metric-value {
    margin-top: 9px;
    font: 700 29px "JetBrains Mono", monospace;
}

.metric-value.green { color: var(--green); }
.metric-value.amber { color: var(--amber); }

.dashboard {
    display: grid;
    grid-template-columns: 1.7fr 1fr;
    gap: 12px;
}

.panel {
    min-height: 420px;
    overflow: hidden;
}

.panel-head {
    height: 54px;
    padding: 0 18px;
    border-bottom: 1px solid var(--border);
    display: flex;
    align-items: center;
    justify-content: space-between;
}

.panel-title {
    font: 12px "JetBrains Mono", monospace;
    color: #aeb8b8;
}

.panel-tag {
    color: var(--green);
    font: 10px "JetBrains Mono", monospace;
    border: 1px solid #164a36;
    background: var(--green-dim);
    padding: 5px 8px;
}

.network {
    position: relative;
    height: 365px;
    overflow: hidden;
}

.network svg {
    position: absolute;
    inset: 0;
    width: 100%;
    height: 100%;
}

.route {
    fill: none;
    stroke: #253232;
    stroke-width: 1.5;
}

.packet {
    fill: var(--green);
    filter: drop-shadow(0 0 5px var(--green));
}

.node {
    position: absolute;
    transform: translate(-50%, -50%);
    min-width: 105px;
    padding: 12px;
    text-align: center;
    border: 1px solid #293333;
    background: #0a0e0e;
    font: 11px "JetBrains Mono", monospace;
}

.node strong {
    display: block;
    color: #dce4e4;
    margin-bottom: 5px;
}

.node small { color: var(--muted); }

.node.server {
    border-color: #0d7049;
    box-shadow: 0 0 28px #00ff8810;
}

.node.server strong { color: var(--green); }

.n1 { left: 14%; top: 22%; }
.n2 { left: 14%; top: 70%; }
.n3 { left: 50%; top: 46%; }
.n4 { left: 85%; top: 22%; }
.n5 { left: 85%; top: 70%; }

.logs {
    height: 365px;
    overflow: auto;
    font: 11px/1.9 "JetBrains Mono", monospace;
    padding: 14px 18px;
}

.log {
    display: grid;
    grid-template-columns: 75px 65px 1fr;
    gap: 8px;
    border-bottom: 1px solid #111818;
    padding: 5px 0;
}

.log-time { color: #4f5a5a; }
.log-method { color: var(--green); }
.log-route { color: #aeb8b8; }

.bottom {
    margin-top: 12px;
    display: grid;
    grid-template-columns: 1.2fr 1fr;
    gap: 12px;
    margin-bottom: 35px;
}

.console {
    padding-bottom: 12px;
}

.console-body {
    padding: 17px 18px;
    min-height: 215px;
    font: 12px/1.9 "JetBrains Mono", monospace;
    color: #849090;
}

.console-body .prompt { color: var(--green); }
.console-body .cmd { color: #e1e8e8; }

.request-bars {
    padding: 22px 18px;
}

.bar-row {
    margin-bottom: 18px;
}

.bar-label {
    display: flex;
    justify-content: space-between;
    color: #899595;
    font: 11px "JetBrains Mono", monospace;
    margin-bottom: 7px;
}

.bar {
    height: 6px;
    background: #171d1d;
}

.bar i {
    display: block;
    height: 100%;
    width: var(--w);
    background: var(--green);
    box-shadow: 0 0 9px #00ff8844;
}

footer {
    border-top: 1px solid var(--border);
    padding: 20px 0 28px;
    display: flex;
    justify-content: space-between;
    color: #4f5959;
    font: 10px "JetBrains Mono", monospace;
}

@media (max-width: 900px) {
    .metrics { grid-template-columns: repeat(2, 1fr); }
    .dashboard, .bottom { grid-template-columns: 1fr; }
}

@media (max-width: 600px) {
    .topline { align-items: start; gap: 20px; flex-direction: column; }
    .metrics { grid-template-columns: 1fr 1fr; }
    .node { min-width: 85px; padding: 9px 5px; font-size: 9px; }
    .log { grid-template-columns: 65px 55px 1fr; font-size: 9px; }
}
</style>
</head>

<body>
<div class="shell">

<header>
    <div class="brand">cpp<span>::</span>server_monitor</div>
    <div class="status">
        <span class="status-dot"></span>
        LISTENING · 0.0.0.0:8080
    </div>
</header>

<section class="topline">
    <div>
        <div class="eyebrow">LIVE NETWORK TELEMETRY</div>
        <h1>Request Flow</h1>
        <div class="subtitle">TCP → accept() → parse() → filesystem → response()</div>
    </div>
    <div class="clock" id="clock">--:--:--</div>
</section>

<section class="metrics">
    <div class="metric">
        <div class="metric-label">Active connections</div>
        <div class="metric-value green" id="connections">4</div>
    </div>
    <div class="metric">
        <div class="metric-label">Requests / sec</div>
        <div class="metric-value" id="rps">18</div>
    </div>
    <div class="metric">
        <div class="metric-label">Avg latency</div>
        <div class="metric-value" id="latency">7 ms</div>
    </div>
    <div class="metric">
        <div class="metric-label">Packets served</div>
        <div class="metric-value amber" id="packets">12,481</div>
    </div>
</section>

<section class="dashboard">

    <div class="panel">
        <div class="panel-head">
            <div class="panel-title">NETWORK / CONNECTION TOPOLOGY</div>
            <div class="panel-tag">LIVE</div>
        </div>

        <div class="network">
            <svg viewBox="0 0 1000 500" preserveAspectRatio="none">
                <path class="route" d="M140 110 L500 230 L850 110"/>
                <path class="route" d="M140 350 L500 230 L850 350"/>

                <circle class="packet" r="5">
                    <animateMotion dur="2.1s" repeatCount="indefinite"
                        path="M140 110 L500 230 L850 110"/>
                </circle>

                <circle class="packet" r="5">
                    <animateMotion dur="1.8s" repeatCount="indefinite"
                        path="M850 350 L500 230 L140 350"/>
                </circle>

                <circle class="packet" r="4">
                    <animateMotion dur="2.7s" repeatCount="indefinite"
                        path="M140 350 L500 230 L850 350"/>
                </circle>
            </svg>

            <div class="node n1">
                <strong>CLIENT_01</strong>
                <small>192.168.1.21</small>
            </div>

            <div class="node n2">
                <strong>CLIENT_02</strong>
                <small>192.168.1.24</small>
            </div>

            <div class="node server n3">
                <strong>C++ SERVER</strong>
                <small>PORT 8080</small>
            </div>

            <div class="node n4">
                <strong>CLIENT_03</strong>
                <small>192.168.1.37</small>
            </div>

            <div class="node n5">
                <strong>CLIENT_04</strong>
                <small>192.168.1.42</small>
            </div>
        </div>
    </div>

    <div class="panel">
        <div class="panel-head">
            <div class="panel-title">REQUEST STREAM</div>
            <div class="panel-tag">TAIL -f</div>
        </div>

        <div class="logs" id="logs">
            <div class="log"><span class="log-time">21:42:01</span><span class="log-method">GET</span><span class="log-route">/index.html</span></div>
            <div class="log"><span class="log-time">21:42:02</span><span class="log-method">GET</span><span class="log-route">/assets/main.css</span></div>
            <div class="log"><span class="log-time">21:42:03</span><span class="log-method">GET</span><span class="log-route">/api/status</span></div>
            <div class="log"><span class="log-time">21:42:04</span><span class="log-method">GET</span><span class="log-route">/projects/http-server</span></div>
            <div class="log"><span class="log-time">21:42:05</span><span class="log-method">GET</span><span class="log-route">/favicon.ico</span></div>
            <div class="log"><span class="log-time">21:42:06</span><span class="log-method">GET</span><span class="log-route">/index.html</span></div>
            <div class="log"><span class="log-time">21:42:07</span><span class="log-method">GET</span><span class="log-route">/metrics</span></div>
        </div>
    </div>

</section>

<section class="bottom">

    <div class="panel console">
        <div class="panel-head">
            <div class="panel-title">SERVER CONSOLE</div>
            <div class="panel-tag">STDOUT</div>
        </div>

        <div class="console-body">
            <div><span class="prompt">$</span> ./httpServer</div>
            <div class="cmd">Creating socket...</div>
            <div class="cmd">Binding port 8080...</div>
            <div class="cmd">Listening for connections...</div>
            <br>
            <div><span class="prompt">[ACCEPT]</span> client fd=7</div>
            <div><span class="prompt">[PARSE]</span> GET /index.html HTTP/1.1</div>
            <div><span class="prompt">[FILE]</span> ./index.html</div>
            <div><span class="prompt">[SEND]</span> 200 OK · 18.4 KB</div>
        </div>
    </div>

    <div class="panel">
        <div class="panel-head">
            <div class="panel-title">REQUEST DISTRIBUTION</div>
            <div class="panel-tag">LAST 60s</div>
        </div>

        <div class="request-bars">
            <div class="bar-row">
                <div class="bar-label"><span>GET /</span><span>82%</span></div>
                <div class="bar"><i style="--w:82%"></i></div>
            </div>
            <div class="bar-row">
                <div class="bar-label"><span>GET /assets</span><span>64%</span></div>
                <div class="bar"><i style="--w:64%"></i></div>
            </div>
            <div class="bar-row">
                <div class="bar-label"><span>GET /api</span><span>41%</span></div>
                <div class="bar"><i style="--w:41%"></i></div>
            </div>
            <div class="bar-row">
                <div class="bar-label"><span>OTHER</span><span>18%</span></div>
                <div class="bar"><i style="--w:18%"></i></div>
            </div>
        </div>
    </div>

</section>

<footer>
    <span>built around sockets · TCP · HTTP · file I/O</span>
    <span id="footerStatus">server healthy</span>
</footer>

</div>

<script>
const routes = [
    "/index.html",
    "/assets/main.css",
    "/api/status",
    "/projects/http-server",
    "/metrics",
    "/favicon.ico"
];

function updateClock() {
    document.getElementById("clock").textContent =
        new Date().toLocaleTimeString("en-US", { hour12: false });
}

function updateMetrics() {
    const connections = 3 + Math.floor(Math.random() * 5);
    const rps = 12 + Math.floor(Math.random() * 18);
    const latency = 4 + Math.floor(Math.random() * 10);
    const packets = 12481 + Math.floor(Math.random() * 70);

    document.getElementById("connections").textContent = connections;
    document.getElementById("rps").textContent = rps;
    document.getElementById("latency").textContent = latency + " ms";
    document.getElementById("packets").textContent = packets.toLocaleString();
}

function addLog() {
    const logs = document.getElementById("logs");
    const now = new Date().toLocaleTimeString("en-US", {
        hour12: false
    });

    const row = document.createElement("div");
    row.className = "log";
    row.innerHTML =
        `<span class="log-time">${now}</span>` +
        `<span class="log-method">GET</span>` +
        `<span class="log-route">${routes[Math.floor(Math.random() * routes.length)]}</span>`;

    logs.prepend(row);

    while (logs.children.length > 10) {
        logs.removeChild(logs.lastChild);
    }
}

setInterval(updateClock, 1000);
setInterval(updateMetrics, 1200);
setInterval(addLog, 1700);

updateClock();
</script>

</body>
</html>
