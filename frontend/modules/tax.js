/* ==========================================
   TAX MODULE — DP with Memoization
   ========================================== */
(function () {
  const slabs = [
    { limit: 250000, rate: 0.0 },
    { limit: 500000, rate: 0.05 },
    { limit: 1000000, rate: 0.20 },
    { limit: 10000000, rate: 0.30 }
  ];
  const MEMO_SIZE = 1000;
  const memo = new Array(MEMO_SIZE).fill(-1);
  let currentViz = 'taxbar';
  let lastBreakdown = null;

  const canvas = document.getElementById('tax-canvas');
  const ctx = canvas.getContext('2d');

  function calculateTax(income) {
    const memoIdx = Math.floor(income / 1000);
    let cacheHit = false;

    if (memoIdx >= 0 && memoIdx < MEMO_SIZE && memo[memoIdx] !== -1) {
      cacheHit = true;
      return { tax: memo[memoIdx], cacheHit, memoIdx, breakdown: null };
    }

    let totalTax = 0, prevLimit = 0;
    const breakdown = [];

    for (let i = 0; i < slabs.length; i++) {
      if (income > slabs[i].limit) {
        const taxable = slabs[i].limit - prevLimit;
        const tax = taxable * slabs[i].rate;
        breakdown.push({ from: prevLimit, to: slabs[i].limit, rate: slabs[i].rate, taxable, tax });
        totalTax += tax;
        prevLimit = slabs[i].limit;
      } else {
        const taxable = income - prevLimit;
        const tax = taxable * slabs[i].rate;
        breakdown.push({ from: prevLimit, to: income, rate: slabs[i].rate, taxable, tax });
        totalTax += tax;
        prevLimit = income;
        break;
      }
    }

    if (income > prevLimit && slabs.length > 0) {
      const taxable = income - prevLimit;
      const tax = taxable * slabs[slabs.length - 1].rate;
      breakdown.push({ from: prevLimit, to: income, rate: slabs[slabs.length - 1].rate, taxable, tax });
      totalTax += tax;
    }

    if (memoIdx >= 0 && memoIdx < MEMO_SIZE) memo[memoIdx] = totalTax;

    return { tax: totalTax, cacheHit, memoIdx, breakdown };
  }

  function formatINR(n) {
    return '₹' + n.toLocaleString('en-IN', { maximumFractionDigits: 0 });
  }

  // --- DRAW SLAB BREAKDOWN ---
  function drawSlabBreakdown() {
    C.clear(ctx, canvas.width, canvas.height);
    if (!lastBreakdown) {
      C.text(ctx, 'Enter income to see tax slab breakdown', canvas.width / 2, canvas.height / 2, 13, '#6b7280');
      return;
    }

    const bd = lastBreakdown.breakdown;
    const total = lastBreakdown.tax;
    const barX = 100, barW = canvas.width - 160, barH = 36, gap = 12;
    const startY = 30;
    const colors = ['#6b7280', '#3b82f6', '#f59e0b', '#ef4444', '#ec4899'];

    bd.forEach((s, i) => {
      const y = startY + i * (barH + gap);
      const pct = total > 0 ? s.tax / total : 0;
      const w = Math.max(pct * barW, 4);

      // Label
      const label = `${formatINR(s.from)} – ${formatINR(s.to)} @ ${(s.rate * 100).toFixed(0)}%`;
      C.text(ctx, label, barX - 8, y + barH / 2, 10, '#9ca3af', 'right');

      // Bar background
      C.roundRect(ctx, barX, y, barW, barH, 6, 'rgba(255,255,255,0.03)', 'rgba(255,255,255,0.05)');

      // Bar fill
      if (s.tax > 0) {
        const color = colors[i % colors.length];
        C.roundRect(ctx, barX, y, w, barH, 6, color + '40', color + '80');
        C.text(ctx, formatINR(s.tax), barX + w + 10, y + barH / 2, 10, color, 'left');
      } else {
        C.text(ctx, '₹0 (exempt)', barX + 10, y + barH / 2, 10, '#6b7280', 'left');
      }
    });

    const totalY = startY + bd.length * (barH + gap) + 20;
    C.roundRect(ctx, barX, totalY, barW, 44, 10, 'rgba(16,185,129,0.1)', 'rgba(16,185,129,0.3)');
    C.text(ctx, `Total Tax: ${formatINR(total)}`, barX + barW / 2, totalY + 15, 16, '#10b981');
    const effRate = lastBreakdown.income > 0 ? (total / lastBreakdown.income * 100).toFixed(2) : 0;
    C.text(ctx, `Effective Rate: ${effRate}%`, barX + barW / 2, totalY + 33, 11, '#6b7280');
  }

  // --- DRAW MEMO CACHE ---
  function drawMemoCache() {
    C.clear(ctx, canvas.width, canvas.height);
    const cellSize = 18, gap = 2;
    const cols = Math.floor((canvas.width - 40) / (cellSize + gap));
    const rows = Math.min(Math.ceil(MEMO_SIZE / cols), Math.floor((canvas.height - 60) / (cellSize + gap)));
    const total = cols * rows;
    const startX = (canvas.width - cols * (cellSize + gap)) / 2;
    const startY = 30;

    C.text(ctx, 'DP Memo Cache (each cell = 1k income bucket)', canvas.width / 2, 14, 11, '#6b7280');

    let filledCount = 0;
    for (let i = 0; i < Math.min(total, MEMO_SIZE); i++) {
      const col = i % cols, row = Math.floor(i / cols);
      const x = startX + col * (cellSize + gap);
      const y = startY + row * (cellSize + gap);
      const filled = memo[i] !== -1;
      if (filled) filledCount++;

      const fill = filled ? 'rgba(16,185,129,0.2)' : 'rgba(255,255,255,0.02)';
      const stroke = filled ? 'rgba(16,185,129,0.4)' : 'rgba(255,255,255,0.04)';
      ctx.fillStyle = fill;
      ctx.strokeStyle = stroke;
      ctx.lineWidth = 0.5;
      ctx.fillRect(x, y, cellSize, cellSize);
      ctx.strokeRect(x, y, cellSize, cellSize);
    }

    C.text(ctx, `${filledCount} / ${MEMO_SIZE} buckets cached`, canvas.width / 2, canvas.height - 16, 11, '#10b981');
  }

  function render() {
    if (currentViz === 'taxbar') drawSlabBreakdown();
    else drawMemoCache();
  }

  // --- EVENTS ---
  document.getElementById('tax-calc-btn').addEventListener('click', () => {
    const income = parseFloat(document.getElementById('tax-income').value);
    if (isNaN(income) || income < 0) {
      consoleLog('tax-console', 'Error: Enter a valid income.', 'warn'); return;
    }

    const result = calculateTax(income);
    if (result.cacheHit) {
      consoleLog('tax-console', `[DP] Cache HIT for income level ${result.memoIdx}k`, 'hit');
      consoleLog('tax-console', `Estimated Tax: ${formatINR(result.tax)}`, 'success');
    } else {
      consoleLog('tax-console', `[DP] Cache MISS — computing tax for ${formatINR(income)}...`, 'miss');
      result.breakdown.forEach(s => {
        consoleLog('tax-console', `  Slab ${formatINR(s.from)}–${formatINR(s.to)} @ ${(s.rate * 100)}%: ${formatINR(s.tax)}`, 'info');
      });
      consoleLog('tax-console', `Estimated Tax: ${formatINR(result.tax)} | Stored in memo[${result.memoIdx}]`, 'success');
    }

    if (result.breakdown) {
      lastBreakdown = { breakdown: result.breakdown, tax: result.tax, income };
    } else {
      // Cache hit — recalc breakdown for display
      const fresh = calculateTax(income + 1); // force fresh for display
      lastBreakdown = { breakdown: fresh.breakdown || [], tax: result.tax, income };
    }
    render();
  });

  document.getElementById('tax-clear-btn').addEventListener('click', () => {
    for (let i = 0; i < MEMO_SIZE; i++) memo[i] = -1;
    consoleLog('tax-console', '[DP] Memo cache cleared.', 'warn');
    lastBreakdown = null;
    render();
  });

  document.addEventListener('vizTabChange', (e) => {
    if (e.detail.module !== 'tax') return;
    currentViz = e.detail.viz; render();
  });

  render();
})();
