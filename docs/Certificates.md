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
