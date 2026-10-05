# Privacy

**GLYPH app** · updated 6 October 2026

## The short version

GLYPH has no accounts. Your messages, tracks and settings stay on your phone and on your GLYPH
device. The only thing that reaches us is what activating a GLYPH needs: its activation code and the
ID of its chip, once (see [Activation](#activation)).

## Bluetooth

The app uses Bluetooth only to talk to your own GLYPH device: to send and receive messages, routes,
books and settings, and to read its battery, position and sensors.

## Your team

GLYPH sends messages, and its position for the team radar and SOS, by radio to other GLYPH devices
in range, never through a server. In secure mode they are encrypted with your team key (AES-256-GCM)
and only your team can read them. In public mode any GLYPH in range can read them.

## Activation

Each GLYPH is activated once, with the code that came with it. The app sends the code and your
GLYPH's chip ID (a 12-character hardware number) to our activation server. The server keeps, for each
code: a scrambled form of the code (SHA-256, not the code itself), the chip ID of the GLYPH it
activated, and the date. It does not keep your IP address, your phone's details or your location. We
use this only to activate GLYPH devices and to stop one code from activating more than one.

The app keeps the code on your phone so that a reset GLYPH can activate again by itself. To have a
code freed from its GLYPH (for example after replacing a broken board), write to us.

## Location

The position shown in the app comes from the GPS inside GLYPH. The app does not track your phone.
On older Android versions the system may ask for location access because it is required to scan for
Bluetooth devices; the app does not read or store the phone's location.

## What stays on your phone

- Messages, recorded tracks and an event log, in the app's local storage.
- Your settings and your activation code.
- The Wi-Fi network GLYPH updates its firmware from, with its password only if you ask the app to remember it.
- Map areas you saved for offline use.

Uninstalling the app or clearing its data removes all of it.

## Online services the app may contact

- **OpenFreeMap** (openfreemap.org), for the detailed map when you look at it or save it with
  internet. The outline map of the whole world is inside the app and needs no connection.
- **Google's map tile servers**, for satellite images, when you choose the satellite view with internet.
- **OpenStreetMap Nominatim**, when a place is not a town in the list inside the app (a street, a peak,
  a hut), and the **OSRM** routing services (FOSSGIS for walking and cycling), when you ask for an
  automatic route. Only the text you typed or the two points are sent.
- **Terrain Tiles** on AWS Open Data, for the heights of a route's area when you send it to GLYPH
  with internet; the app turns them into the contour lines on GLYPH's map.
- **Expo** (expo.dev), to check for and download app updates. The check sends the app's version,
  the phone's system (Android) and a random number made when the app is installed, which is not
  linked to you or your phone and is not shared with us.
- **The GLYPH server**, when you update GLYPH's firmware over Wi-Fi and to activate a GLYPH (see above).

These services receive your IP address like any website does. Apart from what is listed above, GLYPH
sends them nothing.

Map data © OpenStreetMap contributors (OpenFreeMap, OpenMapTiles, Protomaps); places © GeoNames
(CC BY 4.0); heights: Terrain Tiles by Mapzen and others (SRTM, GMTED, NED, ETOPO1 and more).

## Notifications

Notifications are created on your phone from what your GLYPH reports. No push service is used.

## Contact

Questions or requests: [open an issue](https://github.com/Mqr1oo/GLYPH/issues) or write to
glyph.system@gmail.com.
