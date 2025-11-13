protocolo TLS y certificados 
1. ¿Qué es TLS, por qué importa y qué es un certificado aquí?
TLS es un protocolo que da cifrado, autenticación e integridad a las comunicaciones, en pocas palabras: que nadie lea lo que mandas, que nadie lo altere, y que hablas con quien crees que hablas. Importa porque sin eso cualquier intermediario en la red puede espiar o inyectar cosas, y porque muchos clientes y navegadores ya lo exigen como estándar para conexiones seguras tipo HTTPS o MQTT sobre TLS. En este contexto, un “certificado TLS” es un documento digital emitido por una autoridad que incluye la identidad del dominio y la clave pública del servidor, firmado para que el cliente pueda verificar esa identidad durante el handshake.​

2. ¿A qué riesgos te expones si no usas TLS?
A que alguien intercepte el tráfico y lea credenciales o payloads sensibles, típico en Wi‑Fi compartida o cuando el gateway no cifra nada, eso es un MitM de libro. También quedas vulnerable a manipulación de mensajes (integridad rota), por ejemplo cambiar un comando en un topic o inyectar respuestas falsas del broker, y además a suplantación de servidor porque el cliente no valida identidad.​

3. ¿Qué es un CA (Certificate Authority)?
Es una entidad de confianza que valida identidades y firma certificados, básicamente un notario criptográfico que dice “este dominio y esta clave pública sí van juntos”. Los clientes confían en una lista preinstalada de CAs, y si el certificado del servidor está firmado por alguna de esas (directa o mediante intermediarios), la validación pasa y se establece el canal seguro.​

4. ¿Qué es una cadena de certificados y vigencia promedio de sus eslabones?
Es la ruta de confianza desde el certificado del servidor (hoja) hasta un root CA del sistema, normalmente: servidor → uno o más intermediarios → root en el almacén del cliente. En tiempos, los roots viven muchos años (varios hasta 20–25 años), los intermedios duran menos (usualmente 1–5 años), y los certificados de servidor suelen tener vigencias cortas, a veces de meses a 1–2 años según políticas actuales de emisión.​

5. ¿Qué es un keystore y qué es un certificate bundle?
Keystore es un almacén que guarda tu identidad: certificado más su clave privada, se usa para presentarte y firmar, por ejemplo en un servidor o en el cliente si hay autenticación mutua; en Java también existe el truststore que guarda CAs para verificar al otro. Un certificate bundle es el paquetito de certificados concatenados (por ejemplo PEM) que incluye el del servidor y la(s) cadena(s) intermedia(s) para que el cliente pueda construir la ruta hasta un root confiable sin quedarse a medias.​

6. ¿Qué es la autenticación mutua en TLS?
Es cuando no solo el cliente valida al servidor, sino que el servidor también pide y valida el certificado del cliente, así ambos prueban su identidad antes de intercambiar datos. Esto reduce suplantaciones en servicios internos o APIs críticas, porque un atacante no solo tendría que interceptar, también tendría que tener un certificado de cliente válido emitido por una CA de confianza del servidor.​

7. ¿Cómo se habilita la validación de certificados en el ESP32?
La validación de certificados en el ESP32 se habilita al establecer una conexión segura mediante el protocolo TLS. Para hacerlo, se utiliza la librería WiFiClientSecure, que permite incluir un certificado raíz (CA) dentro del código o almacenarlo en la memoria del dispositivo. De esta forma, el ESP32 puede verificar que el servidor con el que se comunica es legítimo y que el certificado ha sido emitido por una autoridad confiable. Esto garantiza una conexión segura y protege los datos frente a ataques de tipo “man-in-the-middle”.

8. Si el sketch necesita conectarse a múltiples dominios con certificados generados por CAs distintos, ¿qué alternativas hay?
Cuando el ESP32 necesita conectarse a varios dominios con diferentes autoridades certificadoras, existen distintas opciones. Una de ellas es incluir todos los certificados raíz necesarios en el código y seleccionar el adecuado según el dominio. Otra alternativa es almacenarlos en el sistema de archivos del dispositivo, lo que facilita su actualización sin recompilar el programa. También es posible usar huellas digitales (fingerprints) del certificado, aunque esta opción requiere cambiar el valor cuando el certificado se renueva. Por último, se puede optar por unificar los certificados bajo una misma autoridad certificadora si los dominios son propios, lo que simplifica la validación.

9. ¿Cómo se puede obtener el certificado para un dominio?
El certificado de un dominio puede obtenerse de diferentes maneras. Una forma sencilla es a través del navegador web, accediendo al sitio HTTPS, haciendo clic en el candado de seguridad y exportando el certificado en formato PEM o DER. También puede generarse mediante servicios gratuitos como Let’s Encrypt, que ofrecen certificados válidos y reconocidos por los navegadores. Si se tiene un dominio propio, la autoridad certificadora proporcionará el archivo correspondiente al momento de emitir el certificado. Ese archivo puede cargarse en el ESP32 para validar las conexiones seguras.

10. ¿A qué se hace referencia cuando se habla de llave pública y privada en el contexto de TLS?
En el contexto de TLS, las llaves pública y privada son un par criptográfico que se utiliza para cifrar y descifrar la información transmitida. La llave pública puede compartirse libremente y sirve para verificar la identidad del servidor o cifrar datos que solo la llave privada puede descifrar. La llave privada, en cambio, se mantiene en secreto y garantiza que solo el servidor legítimo pueda leer o firmar la información. Este mecanismo permite que los datos viajen de forma segura, incluso a través de redes no confiables.

11. ¿Qué pasará con el código cuando los certificados expiren?
Cuando un certificado expira, el ESP32 dejará de considerar válida la conexión y no podrá comunicarse con el servidor de manera segura. Esto provocará errores al intentar establecer conexiones HTTPS hasta que se actualice el certificado. Si el certificado está embebido en el código, será necesario actualizar el firmware; si está guardado en el sistema de archivos, bastará con reemplazar el archivo antiguo por el nuevo. Por esta razón, es importante tener un plan de mantenimiento o renovación de certificados antes de su fecha de vencimiento.

12. ¿Qué teoría matemática es el fundamento de la criptografía moderna? ¿Cuáles son las posibles implicaciones de la computación cuántica para los métodos de criptografía actuales?
La criptografía moderna se basa principalmente en la teoría de números, especialmente en problemas matemáticos difíciles de resolver como la factorización de números primos y el cálculo del logaritmo discreto. Estos problemas son la base de algoritmos como RSA y Diffie-Hellman, que permiten mantener la seguridad de la información en internet. Sin embargo, la aparición de la computación cuántica podría poner en riesgo estos sistemas. Los computadores cuánticos, mediante algoritmos como el de Shor, podrían resolver estos problemas en mucho menos tiempo, haciendo vulnerables los métodos de cifrado actuales. Por eso, se están desarrollando nuevas técnicas de criptografía post-cuántica que buscan resistir los ataques de este tipo de computadoras.

## Prueba de Codigo

## Prueba de conexión segura MQTT (ESP32 + HiveMQ)

### Etapa 1: Puerto seguro (8883) sin certificados
- Se modificó el puerto MQTT a `8883` y se usó `WiFiClientSecure`.
- Resultado: ❌ No conecta, error TLS por falta de certificados.

### Etapa 2: Conexión sin validación (`setInsecure()`)
- Se añadió `espClient.setInsecure();`
- Resultado: ✅ Conecta exitosamente, aunque sin seguridad completa.

### Etapa 3: Con validación de certificados
- Se agregó el certificado raíz de HiveMQ (`root_ca`).
- Resultado: ✅ Conexión estable y segura por TLS.
- Evidencia: Capturas del monitor serie mostrando “MQTT conectado (TLS)”.

---

## Conclusión
Se verificó que el ESP32 requiere certificados válidos para establecer una conexión segura por MQTT.  
La conexión sin validación (etapa 2) funciona pero **no es segura**, mientras que con el certificado cargado (etapa 3) se logra una comunicación **encriptada y autenticada**.
