# 🛡️ Protocolo TLS y Certificados en el ESP32

## 1. ¿Qué es TLS, por qué importa y qué es un certificado aquí?

**TLS (Transport Layer Security)** es un protocolo que proporciona **cifrado, autenticación e integridad** en las comunicaciones.  
En otras palabras:
- Evita que terceros lean los datos transmitidos.  
- Asegura que la información no sea alterada.  
- Verifica que el servidor con el que te comunicas sea realmente quien dice ser.  

Un **certificado TLS** es un documento digital emitido por una **autoridad certificadora (CA)** que asocia la identidad de un dominio con una clave pública. El cliente lo usa para validar la autenticidad del servidor durante el *handshake*.

---

## 2. ¿A qué riesgos te expones si no usas TLS?

Sin TLS, la comunicación queda expuesta a ataques de tipo **“man-in-the-middle” (MitM)**, donde un atacante puede:
- Interceptar credenciales o datos sensibles.  
- Modificar comandos MQTT o inyectar mensajes falsos.  
- Suplantar la identidad del servidor.  

Esto compromete la **confidencialidad, integridad y autenticidad** de toda la comunicación.

---

## 3. ¿Qué es una CA (Certificate Authority)?

Una **CA** es una entidad de confianza que valida identidades y **firma certificados digitales**.  
Actúa como un **notario criptográfico**, certificando que un dominio realmente pertenece a quien dice ser.  
Los sistemas incluyen listas de CAs confiables preinstaladas para verificar estas firmas.

---

## 4. ¿Qué es una cadena de certificados y vigencia promedio de sus eslabones?

Una **cadena de certificados** conecta el certificado del servidor con un **root CA** reconocido:  
`Servidor → Intermediarios → Root CA`

- **Root CA:** vigencia de hasta 20–25 años.  
- **Certificados intermedios:** duran entre 1–5 años.  
- **Certificados de servidor:** normalmente 3 meses a 2 años.  

---

## 5. ¿Qué es un keystore y qué es un certificate bundle?

- **Keystore:** almacena el certificado y su clave privada, usado para identificarse (por ejemplo, un servidor).  
- **Certificate bundle:** archivo que agrupa varios certificados (por ejemplo, en formato `.pem`) para formar la cadena completa de confianza.

---

## 6. ¿Qué es la autenticación mutua en TLS?

Es cuando **tanto el cliente como el servidor presentan y validan certificados**.  
Esto garantiza que ambos sean legítimos antes de intercambiar datos.  
Se usa en **servicios internos o APIs críticas** donde la seguridad es esencial.

---

## 7. ¿Cómo se habilita la validación de certificados en el ESP32?

En el ESP32 se usa la librería **`WiFiClientSecure`**, que permite establecer conexiones cifradas mediante TLS.  
Se puede:
- Incluir el **certificado raíz (CA)** en el código.  
- O almacenarlo en el sistema de archivos del ESP32.  

Esto permite al dispositivo **verificar la identidad del servidor** y proteger la conexión contra ataques MitM.

---

## 8. ¿Qué hacer si el ESP32 se conecta a múltiples dominios con distintas CAs?

Opciones posibles:
- Incluir **varios certificados raíz** y seleccionar el correcto según el dominio.  
- Guardar los certificados en el **sistema de archivos** (para actualizarlos sin recompilar).  
- Usar **huellas digitales (fingerprints)**, aunque deben actualizarse con cada renovación.  
- O unificar los dominios bajo una misma CA si son propios.

---

## 9. ¿Cómo se obtiene el certificado para un dominio?

Formas comunes:
- Desde el **navegador**, accediendo al candado HTTPS y exportando el certificado en formato `.PEM` o `.DER`.  
- Usando **servicios gratuitos como Let’s Encrypt**.  
- O solicitándolo a una **autoridad certificadora** oficial al registrar un dominio.  

Este archivo se carga luego en el ESP32 para establecer conexiones seguras.

---

## 10. ¿Qué son la llave pública y privada en TLS?

- La **llave pública** se comparte libremente y permite cifrar datos o verificar firmas.  
- La **llave privada** se mantiene en secreto y se usa para descifrar datos o firmar mensajes.  

Este par criptográfico garantiza que solo el dueño legítimo pueda leer o firmar la información.

---

## 11. ¿Qué pasa cuando los certificados expiran?

Cuando un certificado expira:
- El ESP32 ya **no considerará válida la conexión** TLS.  
- Las conexiones HTTPS/MQTT fallarán hasta que se actualice el certificado.  

Si el certificado está:
- **Embebido en el código:** se debe recompilar el firmware.  
- **En el sistema de archivos:** basta con reemplazar el archivo.  

Por eso, se recomienda **planificar la renovación periódica** antes del vencimiento.

---

## 12. Fundamento matemático y computación cuántica

La criptografía moderna se basa en **teoría de números**, especialmente en problemas difíciles como:
- **Factorización de primos (RSA)**  
- **Logaritmo discreto (Diffie-Hellman, ECC)**  

La **computación cuántica** podría romper estos esquemas con algoritmos como el de **Shor**, reduciendo drásticamente la dificultad de estos problemas.  
Por ello, se están desarrollando sistemas de **criptografía post-cuántica**, resistentes a futuros ataques cuánticos.

## Prueba de Codigo

## Prueba de conexión segura MQTT (ESP32 + HiveMQ)

### Etapa 1: Puerto seguro (8883) sin certificados
- Se modificó el puerto MQTT a `8883` y se usó `WiFiClientSecure`.
- **Resultado:**
<img width="840" height="370" alt="image" src="https://github.com/user-attachments/assets/3e6c3b23-5d0d-4067-a893-4af07abba572" />


### Etapa 2: Conexión sin validación (`setInsecure()`)
- Se añadió `espClient.setInsecure();`
- **Resultado:** Conecta exitosamente, aunque sin seguridad completa.
- **Evidencia:**
<img width="929" height="437" alt="image" src="https://github.com/user-attachments/assets/42e54ed6-633e-4efe-9f6d-a8be6c5758d6" />


### Etapa 3: Con validación de certificados
- Se agregó el certificado raíz de HiveMQ (`root_ca`).
- **Resultado:** Conexión estable y segura por TLS.
- **Evidencia:**
<img width="747" height="521" alt="image" src="https://github.com/user-attachments/assets/103ca72b-d7df-4a72-9571-a0148bea8892" />


---

## Conclusión
Se verificó que el ESP32 requiere certificados válidos para establecer una conexión segura por MQTT.  
La conexión sin validación (etapa 2) funciona pero **no es segura**, mientras que con el certificado cargado (etapa 3) se logra una comunicación **encriptada y autenticada**.
