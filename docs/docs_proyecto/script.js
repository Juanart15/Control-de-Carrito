const API = "http://127.0.0.1:5000/api/v1";

// Enviar comando al robot
async function sendCommand(command) {
    await fetch(`${API}/move`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ command })
    });
    console.log("Comando enviado:", command);
}

// Actualizar sensores (por ahora no existe en API → solo muestra placeholder)
async function updateSensors() {
    document.getElementById("sensorData").textContent =
        "Esperando datos MQTT...";
}

// Verificar estado de la API
async function checkStatus() {
    try {
        const res = await fetch(`${API}/healthcheck`);
        const data = await res.json();

        document.getElementById("status").textContent =
            data.status === "OK" ? "ONLINE" : "OFFLINE";
    } catch (err) {
        document.getElementById("status").textContent = "OFFLINE";
    }
}

// Intervalos de actualización
setInterval(updateSensors, 1000);
setInterval(checkStatus, 2000);

