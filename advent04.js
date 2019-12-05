// your code goes here
function check(n) {
  let s = n.toString();
  let ok = false;
  for (let i = 0; i < s.length - 1; ++i) {
  	ok = ok || (s[i] == s[i + 1]);
    if (s[i] > s[i + 1])
    {
    	return false;
    }
  }
  return ok;
}

function check2(n) {
  let s = "x" + n.toString() + "xy";
  let ok = false;

  for (let i = 1; i < s.length - 3; ++i) {
    if (s[i] > s[i + 1])
    {
    	return false;
    }
  	ok = ok || (s[i] == s[i + 1] && s[i] != s[i - 1] && s[i + 1] != s[i + 2]);    
  }
  return ok;
}

const iota = (K, N) => Array.from({ length: N }, (v, i) => i + K);
const good = (left, right) => iota(left, right - left).filter(e => check(e));
const good2 = (left, right) => iota(left, right - left).filter(e => check2(e));

//print(check(123456));
//print(iota(3, 4));
//print(good2(206938,679128).slice(0, 100))

print(good(206938,679128).length);
print(good2(206938,679128).length);

