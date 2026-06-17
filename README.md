# Código Máquina Selladora — Fungiterra

Firmware de control embebido para una **máquina selladora industrial**, desarrollado sobre un microcontrolador **ESP32** con Arduino Framework. El sistema automatiza el ciclo completo de sellado térmico de empaques: sujeción del producto mediante ventosas, descenso del pistón de sellado, reposo de estabilización y soplado final de enfriamiento/expulsión, todo coordinado mediante una **máquina de estados finita (FSM)** no bloqueante basada en `millis()`.

Este repositorio se mantiene como evidencia técnica de diseño e implementación de sistemas de control industrial con microcontroladores, para portafolio profesional.

---

## ¿Qué hace este proyecto?

La selladora resuelve un problema típico de automatización industrial a pequeña/mediana escala: convertir un proceso manual y repetitivo (sujetar el empaque, accionar la prensa de sellado, esperar el tiempo correcto, enfriar y liberar) en un ciclo automático, seguro y ajustable por el propio operario sin necesidad de reprogramar el equipo.

El operario interactúa con la máquina mediante:

- Un **botón de inicio** que debe presionarse **dos veces**, en dos momentos distintos del ciclo, como mecanismo de doble confirmación de seguridad antes de activar partes mecánicas (ventosas y pistón).
- Un **botón/sensor de paro de emergencia**, que interrumpe el ciclo en cualquier momento y regresa la máquina al estado de espera.
- Dos **potenciómetros físicos**, que permiten ajustar en caliente, sin tocar el código:
  - el **tiempo de sellado** (cuánto tiempo permanece el pistón abajo aplicando calor/presión), y
  - el **tiempo de soplado** (cuánto tiempo se enfría/expulsa el producto al final del ciclo).
- Una **lámpara piloto verde**, que comunica el estado de la máquina mediante distintos patrones de parpadeo (espera de inicio vs. alarma de paro de emergencia).

Internamente, el firmware nunca usa `delay()` bloqueante: todo el tiempo se gestiona comparando marcas de tiempo (`millis()`) contra los valores de los potenciómetros y constantes de tiempo fijas, lo que permite que la máquina siga respondiendo a la lectura de botones y al paro de emergencia en todo momento, incluso mientras "espera" dentro de un estado.

## ¿Por qué una máquina de estados?

En automatización industrial, una FSM (Finite State Machine) es el patrón de diseño estándar para procesos secuenciales con condiciones de seguridad: cada fase física del proceso (ventosas activas, pistón bajando, soplador encendido, etc.) se modela como un **estado** explícito, y el código solo necesita decidir dos cosas en cada estado: *qué salidas deben estar activas* y *qué condición permite avanzar al siguiente estado*. Esto hace que la lógica sea:

- **Predecible**: en todo momento se sabe exactamente qué está haciendo la máquina y por qué.
- **Segura**: el paro de emergencia puede forzar el regreso al estado inicial desde cualquier punto del ciclo sin dejar actuadores en un estado intermedio peligroso.
- **Fácil de depurar y ampliar**: agregar un nuevo paso al proceso significa agregar un nuevo `case`, sin reescribir la lógica existente.

## Componentes de hardware controlados

| Actuador / Entrada | Rol en el proceso |
|---|---|
| Ventosas | Sujetan el empaque/producto antes y durante el sellado |
| Pistón de sellado | Aplica presión/calor durante el tiempo configurado |
| Soplador | Enfría y/o expulsa el producto al finalizar el ciclo |
| Lámpara piloto verde | Señaliza visualmente el estado de la máquina al operario |
| Botón de inicio (doble pulsación) | Arranca el ciclo con confirmación de seguridad en dos pasos |
| Paro de emergencia | Corta el ciclo y regresa la máquina a reposo |
| Potenciómetro 1 | Ajusta el tiempo de sellado |
| Potenciómetro 2 | Ajusta el tiempo de soplado |

## Estructura del repositorio

```
├── Selladora31_Codigo_Bueno_copy_20250415185654.ino   # setup() + loop() + lógica de la máquina de estados
├── Variables.h                                         # Pines, banderas de control y temporizadores
├── Funcion_Parpadeo.ino                                # Función auxiliar de parpadeo no bloqueante
└── Documentacion_Selladora.md                          # Documentación técnica ampliada (arquitectura, FSM, pines, diagramas)
```

## Documentación técnica ampliada

Este README ofrece una visión general del proyecto. Para el detalle completo —mapa de pines, descripción estado por estado, diagramas de flujo y de estados (Mermaid), variables internas, y notas de mantenimiento/hallazgos técnicos detectados en el código— consulta:

### 📄 [Documentacion_Selladora.md](./Documentacion_Selladora.md)

Ese documento está escrito en formato compatible con **Obsidian** (incluye diagramas Mermaid renderizables) y constituye la referencia técnica completa del firmware.

## Notas

Este es un proyecto de control industrial real, documentado con fines de portafolio. El código refleja el estado funcional de la máquina en producción; las observaciones de mejora y posibles inconsistencias detectadas durante el análisis se documentan de forma transparente en la sección de hallazgos técnicos del documento ampliado.
