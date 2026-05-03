/* ==========================================
   TRANSACTION MODULE — Trie + Segment Tree
   ========================================== */
(function () {
  // --- TRIE ---
  class TrieNode {
    constructor() { this.children = {}; this.isEnd = false; this.category = ''; }
  }
  class Trie {
    constructor() { this.root = new TrieNode(); }
    insert(word, category) {
      let node = this.root;
      for (const ch of word.toLowerCase()) {
        if (!/[a-z]/.test(ch)) continue;
        if (!node.children[ch]) node.children[ch] = new TrieNode();
        node = node.children[ch];
      }
      node.isEnd = true; node.category = category;
    }
    search(word) {
      let node = this.root; this._lastPath = [];
      for (const ch of word.toLowerCase()) {
        if (!/[a-z]/.test(ch)) continue;
        if (!node.children[ch]) return null;
        node = node.children[ch];
        this._lastPath.push(ch);
      }
      return node.isEnd ? node.category : null;
    }
    getTree() {
      const build = (node, ch) => {
        const n = { ch, isEnd: node.isEnd, cat: node.category, children: [] };
        for (const [k, v] of Object.entries(node.children)) n.children.push(build(v, k));
        return n;
      };
      return build(this.root, '◉');
    }
  }

  // --- SEGMENT TREE ---
  const DAYS = 366;
  class SegTree {
    constructor() { this.tree = new Array(4 * DAYS + 4).fill(0); }
    update(day, amt, node = 1, s = 1, e = DAYS) {
      if (s === e) { this.tree[node] += amt; return; }
      const m = (s + e) >> 1;
      if (day <= m) this.update(day, amt, 2 * node, s, m);
      else this.update(day, amt, 2 * node + 1, m + 1, e);
      this.tree[node] = this.tree[2 * node] + this.tree[2 * node + 1];
    }
    query(L, R, node = 1, s = 1, e = DAYS) {
      if (R < s || e < L) return 0;
      if (L <= s && e <= R) return this.tree[node];
      const m = (s + e) >> 1;
      return this.query(L, R, 2 * node, s, m) + this.query(L, R, 2 * node + 1, m + 1, e);
    }
    // Get highlighted nodes for visualization
    queryPath(L, R, node = 1, s = 1, e = DAYS, depth = 0) {
      const res = [];
      if (R < s || e < L) return res;
      res.push({ node, s, e, val: this.tree[node], full: L <= s && e <= R, depth });
      if (s !== e) {
        const m = (s + e) >> 1;
        res.push(...this.queryPath(L, R, 2 * node, s, m, depth + 1));
        res.push(...this.queryPath(L, R, 2 * node + 1, m + 1, e, depth + 1));
      }
      return res;
    }
  }

  // --- STATE ---
  const trie = new Trie();
  const segTree = new SegTree();
  const transactions = [];
  let currentViz = 'trie';

  // Seed vendors
  [['Amazon', 'Shopping'], ['Zomato', 'Food'], ['Swiggy', 'Food'],
   ['Uber', 'Transport'], ['Netflix', 'Entertainment'], ['Flipkart', 'Shopping'],
   ['Ola', 'Transport'], ['Spotify', 'Entertainment']].forEach(([v, c]) => trie.insert(v, c));

  // --- CANVAS ---
  const canvas = document.getElementById('txn-canvas');
  const ctx = canvas.getContext('2d');

  function drawTrie(highlightPath = []) {
    C.clear(ctx, canvas.width, canvas.height);
    const tree = trie.getTree();
    const positions = [];

    function layout(node, x, y, spread, depth) {
      positions.push({ node, x, y, depth });
      const len = node.children.length;
      if (len === 0) return;
      const totalW = (len - 1) * spread;
      node.children.forEach((child, i) => {
        const cx = x - totalW / 2 + i * spread;
        const cy = y + 55;
        positions.push({ from: { x, y }, to: { x: cx, y: cy } }); // edge
        layout(child, cx, cy, Math.max(spread * 0.5, 20), depth + 1);
      });
    }

    layout(tree, canvas.width / 2, 40, 70, 0);

    // Draw edges
    positions.filter(p => p.from).forEach(p => {
      C.line(ctx, p.from.x, p.from.y + 14, p.to.x, p.to.y - 14, 'rgba(16,185,129,0.2)');
    });

    // Draw nodes
    positions.filter(p => p.node).forEach(p => {
      const isHighlighted = highlightPath.includes(p.node.ch);
      const isEnd = p.node.isEnd;
      const fill = isHighlighted ? 'rgba(16,185,129,0.3)' : isEnd ? 'rgba(6,182,212,0.2)' : 'rgba(255,255,255,0.06)';
      const stroke = isHighlighted ? '#10b981' : isEnd ? '#06b6d4' : 'rgba(255,255,255,0.12)';
      C.circle(ctx, p.x, p.y, 14, fill, stroke);
      C.text(ctx, p.node.ch, p.x, p.y, 11, isHighlighted ? '#34d399' : '#f0f0f5');
      if (isEnd) C.text(ctx, p.node.cat, p.x, p.y + 26, 8, '#06b6d4');
    });

    C.text(ctx, 'Vendor → Category Trie', canvas.width / 2, canvas.height - 16, 11, '#6b7280');
  }

  function drawSegTree(rangeL, rangeR) {
    C.clear(ctx, canvas.width, canvas.height);
    const maxDepth = 5; // show top few levels
    const nodeR = 18;

    function draw(node, s, e, x, y, spread, depth) {
      if (depth > maxDepth || s > e) return;
      const val = segTree.tree[node];
      const m = (s + e) >> 1;
      const inRange = rangeL !== undefined && !(rangeR < s || e < rangeL);
      const fullIn = rangeL !== undefined && rangeL <= s && e <= rangeR;

      // Draw children first (lines behind nodes)
      if (s !== e && depth < maxDepth) {
        const lx = x - spread, ly = y + 60;
        const rx = x + spread, ry = y + 60;
        C.line(ctx, x, y + nodeR, lx, ly - nodeR, inRange ? 'rgba(16,185,129,0.3)' : 'rgba(255,255,255,0.08)');
        C.line(ctx, x, y + nodeR, rx, ry - nodeR, inRange ? 'rgba(16,185,129,0.3)' : 'rgba(255,255,255,0.08)');
        draw(2 * node, s, m, lx, ly, spread * 0.48, depth + 1);
        draw(2 * node + 1, m + 1, e, rx, ry, spread * 0.48, depth + 1);
      }

      // Node
      const fill = fullIn ? 'rgba(16,185,129,0.3)' : inRange ? 'rgba(245,158,11,0.15)' : 'rgba(255,255,255,0.04)';
      const stroke = fullIn ? '#10b981' : inRange ? '#f59e0b' : 'rgba(255,255,255,0.1)';
      if (fullIn) C.glow(ctx, x, y, nodeR, 'rgba(16,185,129,0.15)');
      C.circle(ctx, x, y, nodeR, fill, stroke);
      C.text(ctx, val ? val.toFixed(0) : '0', x, y - 2, 9, fullIn ? '#34d399' : '#ccc');
      C.text(ctx, `${s}-${e}`, x, y + 10, 7, '#6b7280');
    }

    draw(1, 1, DAYS, canvas.width / 2, 35, canvas.width * 0.22, 0);
    const label = rangeL !== undefined ? `Range Query [${rangeL}, ${rangeR}]` : 'Segment Tree — Daily Expenses';
    C.text(ctx, label, canvas.width / 2, canvas.height - 16, 11, '#6b7280');
  }

  function render() {
    if (currentViz === 'trie') drawTrie();
    else drawSegTree();
  }

  // --- EVENT HANDLERS ---
  document.getElementById('txn-add-btn').addEventListener('click', () => {
    const vendor = document.getElementById('txn-vendor').value.trim();
    const category = document.getElementById('txn-category').value.trim();
    const amount = parseFloat(document.getElementById('txn-amount').value);
    const day = parseInt(document.getElementById('txn-day').value);

    if (!vendor || isNaN(amount) || isNaN(day) || day < 1 || day > 366) {
      consoleLog('txn-console', 'Error: Please fill all fields correctly.', 'warn');
      return;
    }

    // Trie search
    const matched = trie.search(vendor);
    const finalCat = category || matched || 'Uncategorized';
    if (!category && matched) {
      consoleLog('txn-console', `[Trie] Searched "${vendor}" → found category "${matched}"`, 'info');
      drawTrie(trie._lastPath);
      setTimeout(render, 2000);
    } else if (!category) {
      consoleLog('txn-console', `[Trie] Searched "${vendor}" → not found, using "Uncategorized"`, 'warn');
    }

    transactions.push({ vendor, category: finalCat, amount, day });
    segTree.update(day, amount);

    consoleLog('txn-console', `Added: ${vendor} | ${finalCat} | ₹${amount} | Day ${day}`, 'success');

    // Update table
    const tbody = document.getElementById('txn-table-body');
    const tr = document.createElement('tr');
    tr.innerHTML = `<td>${vendor}</td><td>${finalCat}</td><td>₹${amount}</td><td>${day}</td>`;
    tbody.appendChild(tr);
    document.getElementById('txn-table-wrap').style.display = 'block';

    // Clear inputs
    ['txn-vendor', 'txn-category', 'txn-amount', 'txn-day'].forEach(id => document.getElementById(id).value = '');
  });

  document.getElementById('txn-query-btn').addEventListener('click', () => {
    const L = parseInt(document.getElementById('txn-range-start').value);
    const R = parseInt(document.getElementById('txn-range-end').value);
    if (isNaN(L) || isNaN(R) || L < 1 || R > 366 || L > R) {
      consoleLog('txn-console', 'Error: Invalid range.', 'warn');
      return;
    }
    const sum = segTree.query(L, R);
    consoleLog('txn-console', `[Segment Tree] Range Sum Query [${L}, ${R}]`, 'info');
    consoleLog('txn-console', `Total Expense: ₹${sum.toFixed(2)}`, 'success');
    currentViz = 'segtree';
    document.querySelector('[data-viz="segtree"]').click();
    drawSegTree(L, R);
  });

  document.addEventListener('vizTabChange', (e) => {
    if (e.detail.module !== 'txn') return;
    currentViz = e.detail.viz;
    render();
  });

  // Initial render
  render();
})();
