window.onload = function() {
    x=-0.75;
    y=0;
    s=2.5;
    f=Math.sqrt(2);
    pstep=64;
    debug=1;
    prune_timeout = 5 * 60;
    prune_interval = 60;
    queue_latency_ms = 10;
    active_max = 4;

    dragging=false;
    generation=0;
    canvas = document.getElementById('m');
    cache={};
    queue=[];
    ctx = canvas.getContext('2d');
    ctx.font='12px serif';

    resize();
    compute();
    render(x, y);
    run_queue();

    canvas.addEventListener("mousedown", mousedown);
    canvas.addEventListener("mouseup", mouseup);
    canvas.addEventListener("mousemove", mousemove);
    canvas.addEventListener("mouseleave", mouseleave);
    canvas.addEventListener("dblclick", dblclick);
    canvas.addEventListener("wheel", wheel);
    window.onresize = window_resized;
    window.setInterval(prune, prune_interval * 1000)
}

// Adjust the canvas to match the containing window
function resize() {
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
}

// Compute useful things
function compute() {
    // Find the canvas size
    w = canvas.width;
    h = canvas.height;
    // Find the size in complex plane units
    if(w >= h) {
        // Wider than high (or equal).  Treat s as the height.
        cw = s * w / h;
        ch = s;
    } else {
        // Higher than wide.  Treat s as the width.
        cw = s;
        ch = s * h / w;
    }
    // Find the bottom left corner in complex plane
    cxc = x - cw / 2;
    cyc = y - ch / 2;
    // Find the step in complex plane units
    cstep = pstep * cw / w;
    // Attempt to align to a consistent grid
    cxo = cxc % cstep;
    if(cxo < 0)
        cxo += cstep;
    cxb = cxc - cxo;
    pxs = -Math.floor(cxo * pstep / cstep);
    cyo = cyc % cstep;
    if(cyo < 0)
        cyo += cstep;
    cyb = cyc - cyo;
    pys = -Math.floor(cyo * pstep / cstep);
    if(debug) {
        console.log("pxs:", pxs, "pys:", pys, "w:", w, "h:", h,
                    "pstep:", pstep,
                    "cxb:", cxb, "cyb:", cyb, "cstep: ", cstep);
    }
}

// Render the whole canvas
function render(ccx, ccy) {
    var px, py, pyr, img;
    var cx, cy;
    var n;
    var i, jobs, job;
    n = 0;
    // Start a new generation
    ++generation;
    if(debug > 1) {
        ctx.fillStyle='#00ffff';
        ctx.fillRect(0, 0, w, h);
    }
    // Visit every square in the grid in any old order
    jobs = []
    for(px = pxs, cx = cxb; px < w; px += pstep, cx += cstep) {
        for(pyr = pys, cy = cyb; pyr < h; pyr += pstep, cy += cstep) {
            py = h - pyr - pstep;
            if(debug > 1) {
                console.log("px: ", px, " py: ", py, " cx: ", cx, " cy: ", cy);
                ctx.fillStyle='rgb('+n+','+n+','+n+')';
                ctx.fillRect(px, py, pstep, pstep);
                ctx.fillStyle='#ff0000'
                ctx.fillText(cx + "," + cy, px + 2, py + 18);
                n = (n + 4) % 256;
            }
            job = [x,y,cx,cy,px,py,ccx,ccy,cstep,pstep,generation]
            jobs.push(job);
        }
    }
    jobs.sort(comparator)
    queue = queue.concat(jobs)
}

// Run the queue
function run_queue() {
    var job;
    if(queue.length > 0) {
        active = 0;
        early_skip = 0;
        if(debug)
            console.log("queue run", queue.length);
        while(queue.length > 0 && active < active_max) {
            job = queue.shift();
            run(job);
        }
        if(debug && early_skip > 0)
            console.log("early skip", early_skip);
        run_queue_soon();
    }
}

// Run the queue in a little while
function run_queue_soon() {
    if(queue.length > 0) {
        if(debug)
            console.log("deferring queue run", queue.length);
        window.setTimeout(run_queue, queue_latency_ms);
    }
}

function run(job) {
    var gen = job[10];
    if(gen == generation) {
        var cx = job[2], cy=job[3];
        var px = job[4], py = job[5];
        var cstep = job[8], pstep = job[9];
        var url="/cgi-bin/mandy.fcgi?x="+cx+"&y="+cy+"&s="+cstep+"&w="+pstep+"&h="+pstep;
        var img = url in cache ? cache[url] : new Image();
        img.used = new Date().valueOf();
        if(img.src !== "" && img.complete)
            ctx.drawImage(img, px, py);
        else {
            cache[url] = img;
            img.addEventListener("load", function() {
                if(gen === generation) {
                    ctx.drawImage(img, px, py);
                } else if(debug > 1)
                    console.log("late skip at", px, py);
            }, false);
            if(img.src === "")
                img.src = url;
            ++active;
        }
    } else
        ++early_skip;
}

function distance(job) {
    var x, y, cx, cy, dx, dy;
    cx = job[2];
    cy = job[3];
    ccx = job[6];
    ccy = job[7];
    dx = (ccx - cx);
    dy = (ccy - cy);
    return dx * dx + dy * dy;
}

function comparator(a, b) {
    var da = distance(a);
    var db = distance(b);
    if(da < db)
        return -1;
    else if(da > db)
        return 1;
    else
        return 0;
}

// Scale up or down
function scale(pcx, pcy, ccx, ccy, k) {
    //    cxc = x - cw / 2
    //    ccx = cxc + pcx * cstep / pstep
    //        = cxc + pcx * (pstep * cw / w) / pstep
    //        = cxc + pcx * cw / w
    //        = x - cw/2 + pcx * cw/w
    // =>   x = ccx + cw/2 - pcx * cw/w
    // We want to fix ccx and pcx while replacing x and cw, so:
    //     x' = ccx + cw'/2 - pcx * cw'/w
    //        = ccx + k*cw/2 - k*pcx*cw/w
    //        = ccx + k*cw*(0.5-pcx/w)
    x = ccx + k * cw * (0.5-pcx/w);
    y = ccy + k * ch * (0.5-pcy/h);
    s *= k;
    compute();
    render(ccx, ccy);
    run_queue_soon();
}

// Drag (translate)
function drag(from, to) {
    var xd = to[0] - from[0];
    var yd = to[1] - from[1];
    if(xd == 0 && yd == 0)
        return;
    if(debug)
        console.log("drag by ", xd, " x ", yd);
    x -= xd * cw / w;
    y -= yd * ch / h;
    compute();
    render(x, y);
    run_queue_soon();
}

// Find where a click event landed
function where(e) {
    var rect = e.target.getBoundingClientRect();
    return [e.clientX - rect.left,
            h + rect.top - e.clientY];
}

// Prune cache entries that haven't been used in a while
function prune() {
    var k, i, prunable = [], limit;
    limit = new Date().valueOf() - prune_timeout * 1000;
    for(k in cache)
        if(cache[k].used < limit)
            prunable.push(k);
    if(prunable.length > 0) {
        if(debug)
            console.log("pruning", prunable.length, "items from cache");
        for(i in prunable)
            delete cache[prunable[i]];
        if(debug)
            console.log("cache size now", Object.keys(cache).length);
    }
}

function mousedown(e) {
    draglast = where(e);
    dragging = true;
}

function mousemove(e) {
    if(dragging) {
        dragnew = where(e);
        drag(draglast, dragnew);
        draglast = dragnew;
    }
}

function mouseup(e) {
    dragging = false;
}

function mouseleave(e) {
    dragging = false;
}

function dblclick(e) {
    var clicked = where(e);
    var pcx = clicked[0], pcy = clicked[1];
    var ccx = cxc + pcx * cstep / pstep;
    var ccy = cyc + pcy * cstep / pstep;
    dragging = false;           // just in case
    if(debug)
        console.log("clicked", clicked, [ccx, ccy]);
    scale(pcx, pcy, ccx, ccy, 1/f);
}

function wheel(e) {
    var clicked = where(e);
    dragging = false;
    if(e.deltaY == 0)
        return;
    var pcx = clicked[0], pcy = clicked[1];
    var ccx = cxc + pcx * cstep / pstep;
    var ccy = cyc + pcy * cstep / pstep;
    var k;
    dragging = false;           // just in case
    switch(e.deltaMode) {
    case 0:                     // pixel
        if(Math.abs(e.deltaY) < 20)
            k = Math.pow(f, 0.25);
        else
            k = f;
        break;
    default:
        k = f;
        break;
    }
    if(e.deltaY > 0)
        k = 1/k;
    if(debug)
        console.log("wheel", clicked, [ccx, ccy], k);
    scale(pcx, pcy, ccx, ccy, k);
}

function window_resized(e) {
    console.log("window_resized");
    resize();
    compute();
    render(x, y);
    run_queue_soon();
}
