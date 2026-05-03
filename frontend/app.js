/* ==========================================
   APP.JS — Navigation, Scroll Reveal, Utils
   ========================================== */

// --- Scroll Reveal ---
document.addEventListener('DOMContentLoaded', () => {
  const reveals = document.querySelectorAll('.reveal');
  const observer = new IntersectionObserver((entries) => {
    entries.forEach(e => { if (e.isIntersecting) { e.target.classList.add('visible'); } });
  }, { threshold: 0.12 });
  reveals.forEach(el => observer.observe(el));

  // --- Active Nav Link ---
  const sections = document.querySelectorAll('section[id]');
  const navLinks = document.querySelectorAll('.nav-links a');
  window.addEventListener('scroll', () => {
    let current = '';
    sections.forEach(s => {
      if (window.scrollY >= s.offsetTop - 200) current = s.getAttribute('id');
    });
    navLinks.forEach(a => {
      a.classList.remove('active');
      if (a.getAttribute('href') === '#' + current) a.classList.add('active');
    });
    // Nav background
    document.getElementById('main-nav').style.background =
      window.scrollY > 50 ? 'rgba(6,6,11,0.92)' : 'rgba(6,6,11,0.8)';
  });

  // --- Viz Tab Switching ---
  document.querySelectorAll('.viz-tab').forEach(tab => {
    tab.addEventListener('click', () => {
      const mod = tab.dataset.module;
      tab.closest('.viz-tabs').querySelectorAll('.viz-tab').forEach(t => t.classList.remove('active'));
      tab.classList.add('active');
      // Dispatch custom event for modules to listen to
      document.dispatchEvent(new CustomEvent('vizTabChange', { detail: { module: mod, viz: tab.dataset.viz } }));
    });
  });

  // --- Draw Architecture Diagram ---
  drawArchDiagram();
});

// --- Console Logger Utility ---
function consoleLog(id, msg, type = '') {
  const el = document.getElementById(id);
  const span = document.createElement('span');
  span.className = type ? 'log-' + type : '';
  span.textContent = '$ ' + msg + '\n';
  el.appendChild(span);
  el.scrollTop = el.scrollHeight;
}
function consoleClear(id) {
  document.getElementById(id).innerHTML = '';
}

// --- Canvas Utility Functions ---
const C = {
  clear(ctx, w, h) { ctx.clearRect(0, 0, w, h); },
  circle(ctx, x, y, r, fill, stroke, lw = 2) {
    ctx.beginPath(); ctx.arc(x, y, r, 0, Math.PI * 2);
    if (fill) { ctx.fillStyle = fill; ctx.fill(); }
    if (stroke) { ctx.strokeStyle = stroke; ctx.lineWidth = lw; ctx.stroke(); }
  },
  line(ctx, x1, y1, x2, y2, color = 'rgba(255,255,255,0.15)', lw = 1.5) {
    ctx.beginPath(); ctx.moveTo(x1, y1); ctx.lineTo(x2, y2);
    ctx.strokeStyle = color; ctx.lineWidth = lw; ctx.stroke();
  },
  text(ctx, txt, x, y, size = 12, color = '#f0f0f5', align = 'center', font = 'Inter') {
    ctx.fillStyle = color; ctx.font = `${size}px '${font}', sans-serif`;
    ctx.textAlign = align; ctx.textBaseline = 'middle'; ctx.fillText(txt, x, y);
  },
  roundRect(ctx, x, y, w, h, r, fill, stroke) {
    ctx.beginPath(); ctx.moveTo(x + r, y);
    ctx.lineTo(x + w - r, y); ctx.quadraticCurveTo(x + w, y, x + w, y + r);
    ctx.lineTo(x + w, y + h - r); ctx.quadraticCurveTo(x + w, y + h, x + w - r, y + h);
    ctx.lineTo(x + r, y + h); ctx.quadraticCurveTo(x, y + h, x, y + h - r);
    ctx.lineTo(x, y + r); ctx.quadraticCurveTo(x, y, x + r, y);
    ctx.closePath();
    if (fill) { ctx.fillStyle = fill; ctx.fill(); }
    if (stroke) { ctx.strokeStyle = stroke; ctx.lineWidth = 1.5; ctx.stroke(); }
  },
  glow(ctx, x, y, r, color) {
    const grad = ctx.createRadialGradient(x, y, 0, x, y, r * 2.5);
    grad.addColorStop(0, color); grad.addColorStop(1, 'transparent');
    ctx.fillStyle = grad; ctx.beginPath(); ctx.arc(x, y, r * 2.5, 0, Math.PI * 2); ctx.fill();
  }
};

// --- Architecture Diagram ---
function drawArchDiagram() {
  const canvas = document.getElementById('arch-canvas');
  if (!canvas) return;
  const ctx = canvas.getContext('2d');
  const w = canvas.width, h = canvas.height;
  C.clear(ctx, w, h);

  // Main box
  C.roundRect(ctx, 40, 20, w - 80, 60, 12, 'rgba(16,185,129,0.1)', 'rgba(16,185,129,0.3)');
  C.text(ctx, 'main.c — Menu-Driven Integration', w / 2, 50, 14, '#10b981');

  // Module boxes
  const modules = [
    { label: 'transaction.c', sub: 'Trie + Segment Tree', color: '#10b981', x: 40 },
    { label: 'portfolio.c', sub: 'RB-Tree + Max Heap', color: '#3b82f6', x: 155 },
    { label: 'bills.c', sub: 'Min Heap', color: '#f59e0b', x: 270 },
    { label: 'tax.c', sub: 'DP + Memo', color: '#8b5cf6', x: 385 }
  ];

  modules.forEach((m, i) => {
    const bx = m.x, by = 120, bw = 105, bh = 70;
    // Connection line
    C.line(ctx, bx + bw / 2, 80, bx + bw / 2, by, `${m.color}44`);
    C.roundRect(ctx, bx, by, bw, bh, 10, `${m.color}15`, `${m.color}40`);
    C.text(ctx, m.label, bx + bw / 2, by + 25, 11, m.color);
    C.text(ctx, m.sub, bx + bw / 2, by + 48, 9, '#9ca3af');
  });

  // Header files
  C.roundRect(ctx, 40, 230, w - 80, 50, 10, 'rgba(255,255,255,0.03)', 'rgba(255,255,255,0.08)');
  C.text(ctx, 'include/ — transaction.h  ·  portfolio.h  ·  bills.h  ·  tax.h', w / 2, 255, 11, '#6b7280');

  // Data structures layer
  const ds = [
    { label: 'Trie', color: '#10b981' }, { label: 'Seg Tree', color: '#10b981' },
    { label: 'RB-Tree', color: '#3b82f6' }, { label: 'Max Heap', color: '#3b82f6' },
    { label: 'Min Heap', color: '#f59e0b' }, { label: 'DP Table', color: '#8b5cf6' }
  ];
  const dsY = 320, dsW = 68, dsGap = 6;
  const totalDsW = ds.length * dsW + (ds.length - 1) * dsGap;
  const dsStartX = (w - totalDsW) / 2;
  ds.forEach((d, i) => {
    const dx = dsStartX + i * (dsW + dsGap);
    C.roundRect(ctx, dx, dsY, dsW, 36, 8, `${d.color}12`, `${d.color}30`);
    C.text(ctx, d.label, dx + dsW / 2, dsY + 18, 10, d.color);
  });

  C.text(ctx, 'Data Structures Layer', w / 2, dsY - 16, 10, '#6b7280');
}
