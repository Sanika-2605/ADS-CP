/* ==========================================
   BILLS MODULE — Min Heap Priority Queue
   ========================================== */
(function () {
  class MinHeap {
    constructor() { this.items = []; }

    push(name, amount, date) {
      this.items.push({ name, amount, date });
      let i = this.items.length - 1;
      while (i > 0) {
        const p = Math.floor((i - 1) / 2);
        if (this.items[i].date >= this.items[p].date) break;
        [this.items[i], this.items[p]] = [this.items[p], this.items[i]];
        i = p;
      }
      return this.items.length - 1;
    }

    pop() {
      if (this.items.length === 0) return null;
      const root = this.items[0];
      const last = this.items.pop();
      if (this.items.length > 0) {
        this.items[0] = last;
        let i = 0;
        while (true) {
          let smallest = i;
          const l = 2 * i + 1, r = 2 * i + 2;
          if (l < this.items.length && this.items[l].date < this.items[smallest].date) smallest = l;
          if (r < this.items.length && this.items[r].date < this.items[smallest].date) smallest = r;
          if (smallest === i) break;
          [this.items[i], this.items[smallest]] = [this.items[smallest], this.items[i]];
          i = smallest;
        }
      }
      return root;
    }

    peek() { return this.items.length > 0 ? this.items[0] : null; }
  }

  // --- STATE ---
  const heap = new MinHeap();
  let currentViz = 'heaptree';
  let highlightIdx = -1;

  const canvas = document.getElementById('bill-canvas');
  const ctx = canvas.getContext('2d');

  function formatDate(d) {
    if (!d) return '';
    const parts = d.split('-');
    if (parts.length === 3) return `${parts[2]}/${parts[1]}`;
    return d;
  }

  // --- DRAW HEAP AS TREE ---
  function drawHeapTree() {
    C.clear(ctx, canvas.width, canvas.height);
    const items = heap.items;
    if (items.length === 0) {
      C.text(ctx, 'Add bills to see the Min Heap', canvas.width / 2, canvas.height / 2, 13, '#6b7280');
      return;
    }

    const nodeR = 24;
    function drawNode(i, x, y, spread) {
      if (i >= items.length) return;
      const li = 2 * i + 1, ri = 2 * i + 2;
      if (li < items.length) {
        C.line(ctx, x, y + nodeR, x - spread, y + 65 - nodeR, 'rgba(245,158,11,0.2)');
        drawNode(li, x - spread, y + 65, spread * 0.48);
      }
      if (ri < items.length) {
        C.line(ctx, x, y + nodeR, x + spread, y + 65 - nodeR, 'rgba(245,158,11,0.2)');
        drawNode(ri, x + spread, y + 65, spread * 0.48);
      }

      const isRoot = i === 0;
      const isHL = i === highlightIdx;
      const fill = isRoot ? 'rgba(245,158,11,0.25)' : isHL ? 'rgba(16,185,129,0.25)' : 'rgba(255,255,255,0.05)';
      const stroke = isRoot ? '#f59e0b' : isHL ? '#10b981' : 'rgba(255,255,255,0.12)';
      if (isRoot) C.glow(ctx, x, y, nodeR, 'rgba(245,158,11,0.15)');
      C.circle(ctx, x, y, nodeR, fill, stroke);
      C.text(ctx, items[i].name.substring(0, 8), x, y - 5, 9, '#f0f0f5', 'center', 'JetBrains Mono');
      C.text(ctx, formatDate(items[i].date), x, y + 8, 8, isRoot ? '#f59e0b' : '#9ca3af');
    }

    drawNode(0, canvas.width / 2, 40, canvas.width * 0.22);
    C.text(ctx, 'Min Heap — Earliest Due at Root', canvas.width / 2, canvas.height - 16, 11, '#6b7280');
  }

  // --- DRAW HEAP AS ARRAY ---
  function drawHeapArray() {
    C.clear(ctx, canvas.width, canvas.height);
    const items = heap.items;
    if (items.length === 0) {
      C.text(ctx, 'Add bills to see the heap array', canvas.width / 2, canvas.height / 2, 13, '#6b7280');
      return;
    }

    const cellW = 80, cellH = 60, gap = 6;
    const cols = Math.min(items.length, Math.floor((canvas.width - 40) / (cellW + gap)));
    const rows = Math.ceil(items.length / cols);
    const startX = (canvas.width - cols * (cellW + gap)) / 2;
    const startY = (canvas.height - rows * (cellH + gap)) / 2 - 10;

    items.forEach((item, i) => {
      const col = i % cols, row = Math.floor(i / cols);
      const x = startX + col * (cellW + gap);
      const y = startY + row * (cellH + gap);
      const isRoot = i === 0;
      C.roundRect(ctx, x, y, cellW, cellH, 8,
        isRoot ? 'rgba(245,158,11,0.15)' : 'rgba(255,255,255,0.04)',
        isRoot ? 'rgba(245,158,11,0.3)' : 'rgba(255,255,255,0.08)');
      C.text(ctx, `[${i}]`, x + 14, y + 12, 8, '#6b7280', 'center', 'JetBrains Mono');
      C.text(ctx, item.name.substring(0, 10), x + cellW / 2, y + 28, 10, '#f0f0f5');
      C.text(ctx, `₹${item.amount}`, x + cellW / 2, y + 42, 9, '#f59e0b');
      C.text(ctx, formatDate(item.date), x + cellW / 2, y + 54, 8, '#9ca3af');
    });
    C.text(ctx, 'Heap Array Representation', canvas.width / 2, canvas.height - 16, 11, '#6b7280');
  }

  function render() {
    highlightIdx = -1;
    if (currentViz === 'heaptree') drawHeapTree();
    else drawHeapArray();
  }

  function updateTable() {
    const tbody = document.getElementById('bill-table-body');
    tbody.innerHTML = '';
    heap.items.forEach((b, i) => {
      const tr = document.createElement('tr');
      tr.innerHTML = `<td>${i === 0 ? '⚡ ' : ''}${b.name}</td><td>₹${b.amount}</td><td>${b.date}</td>`;
      tbody.appendChild(tr);
    });
    document.getElementById('bill-table-wrap').style.display = heap.items.length > 0 ? 'block' : 'none';
  }

  // --- EVENTS ---
  document.getElementById('bill-add-btn').addEventListener('click', () => {
    const name = document.getElementById('bill-name').value.trim();
    const amount = parseFloat(document.getElementById('bill-amount').value);
    const date = document.getElementById('bill-date').value;
    if (!name || isNaN(amount) || !date) {
      consoleLog('bill-console', 'Error: Fill all fields.', 'warn'); return;
    }
    heap.push(name, amount, date);
    consoleLog('bill-console', `[Min Heap] Added "${name}" — ₹${amount} due ${date}`, 'success');
    consoleLog('bill-console', `[Min Heap] Next due: ${heap.peek().name} (${heap.peek().date})`, 'info');
    updateTable(); render();
    ['bill-name', 'bill-amount', 'bill-date'].forEach(id => document.getElementById(id).value = '');
  });

  document.getElementById('bill-pay-btn').addEventListener('click', () => {
    const paid = heap.pop();
    if (!paid) { consoleLog('bill-console', 'No bills to pay.', 'warn'); return; }
    consoleLog('bill-console', `[Min Heap] Paid "${paid.name}": ₹${paid.amount} (Due: ${paid.date})`, 'success');
    if (heap.peek()) consoleLog('bill-console', `[Min Heap] Next due: ${heap.peek().name} (${heap.peek().date})`, 'info');
    else consoleLog('bill-console', 'All bills paid! 🎉', 'success');
    updateTable(); render();
  });

  document.getElementById('bill-preload-btn').addEventListener('click', () => {
    const samples = [
      ['Electricity', 2100, '2026-05-10'], ['Internet', 999, '2026-05-05'],
      ['Rent', 15000, '2026-06-01'], ['Insurance', 4500, '2026-05-15'],
      ['Phone', 599, '2026-05-08'], ['Water', 350, '2026-05-20']
    ];
    samples.forEach(([n, a, d]) => {
      heap.push(n, a, d);
      consoleLog('bill-console', `[Min Heap] Added "${n}" due ${d}`, 'info');
    });
    consoleLog('bill-console', `Next due: ${heap.peek().name} (${heap.peek().date})`, 'success');
    updateTable(); render();
  });

  document.addEventListener('vizTabChange', (e) => {
    if (e.detail.module !== 'bill') return;
    currentViz = e.detail.viz; render();
  });

  render();
})();
