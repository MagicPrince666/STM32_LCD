var out;
for (i = 0; i<72; i++) {
    out = 120 - 100*Math.sin(i/36*Math.PI);
    console.log(Math.round(out));
}
