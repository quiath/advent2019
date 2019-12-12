-- Problem2



initialtest = {
{-1, 0, 2},
{2, -10, -7},
{4, -8, 8},
{3, 5, -1}
}

initial = {
{-2, 9, -5},
{16, 19, 9},
{0, 3, 6},
{11, 0, 11}
}

function vcopy(s, r)
    d={}
    for i=1,#s do
        v=s[i]
        d[#d+1]={r*v[1],r*v[2],r*v[3]}
    end
    return d
end

function energy(p,v)
    r,q=0,0
    for k=1,3 do
        r=r+math.abs(p[k])
        q=q+math.abs(v[k])
    end
    return r*q
end

function part1()
    steps = 1000
    p=vcopy(initial, 1)
    v=vcopy(initial, 0)
    n=#p
    print(n)
    for step=1,steps do
        for i=1,n-1 do
            for j=i+1,n do
                --print(i, j)
                --s=""
                for k=1,3 do
                    --s=s..tostring(p[i][k]).." "
                    if p[i][k] < p[j][k] then
                        v[i][k] = v[i][k] + 1
                        v[j][k] = v[j][k] - 1                     
                    elseif p[i][k] > p[j][k] then
                        v[i][k] = v[i][k] - 1
                        v[j][k] = v[j][k] + 1                          
                    end
                end
                --print(s)
            end
        end
        
        for i=1,n do       
            s=""
            for k=1,3 do
                p[i][k] = p[i][k] + v[i][k]
                s=s..tostring(p[i][k]).." "
            end
            --print(step,i,s)
        end
        
    end
    e=0
    for i=1,n do  
        e = e + energy(p[i],v[i])
    end
    print("energy",e)
end

part1()
