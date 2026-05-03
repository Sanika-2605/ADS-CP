/* ==========================================
   PORTFOLIO MODULE — Red-Black Tree + Max Heap
   ========================================== */
(function () {
  const RED = 'red', BLACK = 'black';

  class RBNode {
    constructor(symbol, qty, buy, current) {
      this.symbol = symbol; this.qty = qty; this.buy = buy; this.current = current;
      this.gain = ((current - buy) / buy * 100);
      this.color = RED; this.left = null; this.right = null; this.parent = null;
    }
  }

  class RBTree {
    constructor() { this.NIL = new RBNode('', 0, 0, 0); this.NIL.color = BLACK; this.root = this.NIL; }

    leftRotate(x) {
      const y = x.right; x.right = y.left;
      if (y.left !== this.NIL) y.left.parent = x;
      y.parent = x.parent;
      if (x.parent === this.NIL) this.root = y;
      else if (x === x.parent.left) x.parent.left = y;
      else x.parent.right = y;
      y.left = x; x.parent = y;
    }
    rightRotate(y) {
      const x = y.left; y.left = x.right;
      if (x.right !== this.NIL) x.right.parent = y;
      x.parent = y.parent;
      if (y.parent === this.NIL) this.root = x;
      else if (y === y.parent.left) y.parent.left = x;
      else y.parent.right = x;
      x.right = y; y.parent = x;
    }

    insert(symbol, qty, buy, current) {
      // Check for existing
      let ex = this.search(symbol);
      if (ex !== this.NIL) {
        ex.qty = qty; ex.buy = buy; ex.current = current;
        ex.gain = ((current - buy) / buy * 100);
        return 'updated';
      }
      const z = new RBNode(symbol, qty, buy, current);
      z.left = this.NIL; z.right = this.NIL;
      let y = this.NIL, x = this.root;
      while (x !== this.NIL) { y = x; x = symbol < x.symbol ? x.left : x.right; }
      z.parent = y;
      if (y === this.NIL) this.root = z;
      else if (symbol < y.symbol) y.left = z;
      else y.right = z;
      this.insertFixup(z);
      return 'inserted';
    }

    insertFixup(z) {
      while (z.parent.color === RED) {
        if (z.parent === z.parent.parent.left) {
          const y = z.parent.parent.right;
          if (y.color === RED) {
            z.parent.color = BLACK; y.color = BLACK;
            z.parent.parent.color = RED; z = z.parent.parent;
          } else {
            if (z === z.parent.right) { z = z.parent; this.leftRotate(z); }
            z.parent.color = BLACK; z.parent.parent.color = RED;
            this.rightRotate(z.parent.parent);
          }
        } else {
          const y = z.parent.parent.left;
          if (y.color === RED) {
            z.parent.color = BLACK; y.color = BLACK;
            z.parent.parent.color = RED; z = z.parent.parent;
          } else {
            if (z === z.parent.left) { z = z.parent; this.rightRotate(z); }
            z.parent.color = BLACK; z.parent.parent.color = RED;
            this.leftRotate(z.parent.parent);
          }
        }
      }
      this.root.color = BLACK;
    }

    search(symbol) {
      let n = this.root;
      while (n !== this.NIL && symbol !== n.symbol) {
        n = symbol < n.symbol ? n.left : n.right;
      }
      return n;
    }

    inOrder(node, arr = []) {
      if (node === this.NIL) return arr;
      this.inOrder(node.left, arr); arr.push(node); this.inOrder(node.right, arr);
      return arr;
    }
  }

  // --- MAX HEAP ---
  class MaxHeap {
    constructor() { this.items = []; }
    push(item) {
      this.items.push(item); let i = this.items.length - 1;
      while (i > 0) {
        const p = Math.floor((i - 1) / 2);
        if (this.items[i].gain <= this.items[p].gain) break;
        [this.items[i], this.items[p]] = [this.items[p], this.items[i]]; i = p;
      }
    }
    peek() { return this.items.length > 0 ? this.items[0] : null; }
    buildFrom(arr) { this.items = []; arr.forEach(a => this.push({ symbol: a.symbol, gain: a.gain })); }
  }

  // --- STATE ---
  const tree = new RBTree();
  const heap = new MaxHeap();
  let currentViz = 'rbtree';

  const canvas = document.getElementById('pf-canvas');
  const ctx = canvas.getContext('2d');

  // --- DRAW RB-TREE ---
  function drawRBTree(highlightSymbol) {
    C.clear(ctx, canvas.width, canvas.height);
    if (tree.root === tree.NIL) {
      C.text(ctx, 'Add holdings to see the Red-Black Tree', canvas.width / 2, canvas.height / 2, 13, '#6b7280');
      return;
    }

    function getDepth(n) { return n === tree.NIL ? 0 : 1 + Math.max(getDepth(n.left), getDepth(n.right)); }
    const depth = getDepth(tree.root);
    const nodeR = 22;

    function draw(node, x, y, spread) {
      if (node === tree.NIL) return;
      // Lines to children
      if (node.left !== tree.NIL) {
        C.line(ctx, x, y + nodeR, x - spread, y + 65 - nodeR, 'rgba(255,255,255,0.1)');
        draw(node.left, x - spread, y + 65, spread * 0.52);
      }
      if (node.right !== tree.NIL) {
        C.line(ctx, x, y + nodeR, x + spread, y + 65 - nodeR, 'rgba(255,255,255,0.1)');
        draw(node.right, x + spread, y + 65, spread * 0.52);
      }
      // Node
      const isRed = node.color === RED;
      const isHL = highlightSymbol && node.symbol === highlightSymbol;
      const fill = isRed ? 'rgba(239,68,68,0.25)' : 'rgba(100,100,120,0.25)';
      const stroke = isRed ? '#ef4444' : '#9ca3af';
      if (isHL) C.glow(ctx, x, y, nodeR, 'rgba(16,185,129,0.25)');
      C.circle(ctx, x, y, nodeR, fill, isHL ? '#10b981' : stroke, isHL ? 3 : 2);
      C.text(ctx, node.symbol, x, y - 3, 10, '#f0f0f5', 'center', 'JetBrains Mono');
      C.text(ctx, node.gain.toFixed(1) + '%', x, y + 10, 8, node.gain >= 0 ? '#34d399' : '#ef4444');
    }

    draw(tree.root, canvas.width / 2, 40, Math.min(canvas.width * 0.25, 160));
    C.text(ctx, 'Red-Black Tree (sorted by symbol)', canvas.width / 2, canvas.height - 16, 11, '#6b7280');
  }

  // --- DRAW MAX HEAP ---
  function drawMaxHeap() {
    C.clear(ctx, canvas.width, canvas.height);
    const items = heap.items;
    if (items.length === 0) {
      C.text(ctx, 'Add holdings to see the Max Heap', canvas.width / 2, canvas.height / 2, 13, '#6b7280');
      return;
    }

    // Tree view
    const nodeR = 22;
    function drawNode(i, x, y, spread) {
      if (i >= items.length) return;
      const li = 2 * i + 1, ri = 2 * i + 2;
      if (li < items.length) {
        C.line(ctx, x, y + nodeR, x - spread, y + 60 - nodeR, 'rgba(59,130,246,0.15)');
        drawNode(li, x - spread, y + 60, spread * 0.48);
      }
      if (ri < items.length) {
        C.line(ctx, x, y + nodeR, x + spread, y + 60 - nodeR, 'rgba(59,130,246,0.15)');
        drawNode(ri, x + spread, y + 60, spread * 0.48);
      }
      const isTop = i === 0;
      const fill = isTop ? 'rgba(16,185,129,0.25)' : 'rgba(59,130,246,0.15)';
      const stroke = isTop ? '#10b981' : 'rgba(59,130,246,0.4)';
      if (isTop) C.glow(ctx, x, y, nodeR, 'rgba(16,185,129,0.2)');
      C.circle(ctx, x, y, nodeR, fill, stroke);
      C.text(ctx, items[i].symbol, x, y - 3, 10, '#f0f0f5', 'center', 'JetBrains Mono');
      C.text(ctx, items[i].gain.toFixed(1) + '%', x, y + 10, 8, '#34d399');
    }

    drawNode(0, canvas.width / 2, 40, canvas.width * 0.2);

    // Array view at bottom
    const cellW = 56, cellH = 28;
    const arrY = canvas.height - 60;
    const startX = (canvas.width - items.length * (cellW + 4)) / 2;
    items.forEach((item, i) => {
      const x = startX + i * (cellW + 4);
      const isTop = i === 0;
      C.roundRect(ctx, x, arrY, cellW, cellH, 6,
        isTop ? 'rgba(16,185,129,0.15)' : 'rgba(255,255,255,0.04)',
        isTop ? 'rgba(16,185,129,0.3)' : 'rgba(255,255,255,0.08)');
      C.text(ctx, item.symbol, x + cellW / 2, arrY + cellH / 2, 9, '#ccc', 'center', 'JetBrains Mono');
    });
    C.text(ctx, 'Max Heap — Top Gainer at Root', canvas.width / 2, canvas.height - 16, 11, '#6b7280');
  }

  function render() {
    if (currentViz === 'rbtree') drawRBTree();
    else drawMaxHeap();
  }

  function updateTable() {
    const holdings = tree.inOrder(tree.root);
    const tbody = document.getElementById('pf-table-body');
    tbody.innerHTML = '';
    holdings.forEach(h => {
      const tr = document.createElement('tr');
      const gc = h.gain >= 0 ? 'gain-positive' : 'gain-negative';
      tr.innerHTML = `<td>${h.symbol}</td><td>${h.qty}</td><td>₹${h.buy}</td><td>₹${h.current}</td><td class="${gc}">${h.gain.toFixed(2)}%</td>`;
      tbody.appendChild(tr);
    });
    document.getElementById('pf-table-wrap').style.display = holdings.length > 0 ? 'block' : 'none';
  }

  // --- EVENTS ---
  document.getElementById('pf-add-btn').addEventListener('click', () => {
    const symbol = document.getElementById('pf-symbol').value.trim().toUpperCase();
    const qty = parseInt(document.getElementById('pf-qty').value);
    const buy = parseFloat(document.getElementById('pf-buy').value);
    const current = parseFloat(document.getElementById('pf-current').value);
    if (!symbol || isNaN(qty) || isNaN(buy) || isNaN(current)) {
      consoleLog('pf-console', 'Error: Fill all fields.', 'warn'); return;
    }
    const result = tree.insert(symbol, qty, buy, current);
    const gain = ((current - buy) / buy * 100).toFixed(2);
    consoleLog('pf-console', `[RB-Tree] ${result === 'updated' ? 'Updated' : 'Inserted'} ${symbol} | Gain: ${gain}%`, 'success');
    consoleLog('pf-console', `[RB-Tree] Root: ${tree.root.symbol} (${tree.root.color})`, 'info');

    // Rebuild heap
    heap.buildFrom(tree.inOrder(tree.root));
    updateTable(); render();
    drawRBTree(symbol);
    setTimeout(() => render(), 1500);
    ['pf-symbol', 'pf-qty', 'pf-buy', 'pf-current'].forEach(id => document.getElementById(id).value = '');
  });

  document.getElementById('pf-top-btn').addEventListener('click', () => {
    const top = heap.peek();
    if (!top) { consoleLog('pf-console', 'Portfolio empty.', 'warn'); return; }
    consoleLog('pf-console', `[Max Heap] Top Gainer: ${top.symbol} with ${top.gain.toFixed(2)}% gain`, 'success');
    currentViz = 'maxheap';
    document.querySelector('[data-viz="maxheap"]').click();
  });

  document.getElementById('pf-preload-btn').addEventListener('click', () => {
    const samples = [
      ['AAPL', 10, 150, 172], ['GOOGL', 5, 2800, 2950], ['TSLA', 8, 700, 680],
      ['MSFT', 15, 300, 340], ['AMZN', 3, 3400, 3550], ['NFLX', 12, 500, 475]
    ];
    samples.forEach(([s, q, b, c]) => {
      tree.insert(s, q, b, c);
      consoleLog('pf-console', `[RB-Tree] Inserted ${s} (${((c - b) / b * 100).toFixed(1)}%)`, 'info');
    });
    heap.buildFrom(tree.inOrder(tree.root));
    updateTable(); render();
    consoleLog('pf-console', 'Loaded 6 sample holdings.', 'success');
  });

  document.addEventListener('vizTabChange', (e) => {
    if (e.detail.module !== 'pf') return;
    currentViz = e.detail.viz; render();
  });

  render();
})();
