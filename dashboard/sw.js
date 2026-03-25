const CACHE_NAME = 'glyph-v2';
const ASSETS = [
  './',
  './glyph.html',     
  './manifest.json',
  './icon-192.png'    // Foarte important să fie aici pentru notificări/offline
];

self.addEventListener('install', (e) => {
  e.waitUntil(
    caches.open(CACHE_NAME).then((cache) => cache.addAll(ASSETS))
  );
});

self.addEventListener('fetch', (e) => {
  e.respondWith(
    caches.match(e.request).then((res) => res || fetch(e.request))
  );
});
