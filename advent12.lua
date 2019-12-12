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
    local d={}
    for i=1,#s do
        local v=s[i]
        d[#d+1]={r*v[1],r*v[2],r*v[3]}
    end
    return d
end

function energy(p,v)
    local r,q=0,0
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

function do_step(p, v)
    local n = #p
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

function get_repeating_coord(k)
    -- k is the coord 1, 2, 3 for which we test the repeating state
    local steps = 100000000
    local p=vcopy(initial, 1)
    local v=vcopy(initial, 0)
    
    local p2=vcopy(initial, 1)
    local v2=vcopy(initial, 0)

    local p3 = nil
    local v3 = nil

    local n=#p

    local phase = 0
    local prev = 0
    for step=1,steps do
        do_step(p, v)
        if phase == 0 then
            do_step(p2, v2)
            do_step(p2, v2)
        end
        local ok = true
        for i = 1, n do
            if p[i][k] ~= p2[i][k] or v[i][k] ~= v2[i][k] then
                ok = false
                break
            end
        end
        if ok then
            print(k, step, step - prev)
            phase = phase + 1
            if phase == 2 then
                return prev, step - prev
            end
            prev = step
        end
    end
    return 0, 0
 end

function lcm(a)
    local res = 1
    local n = 1000000
    local p = { 0 }
    for i = 2, n do
        p[i] = 1
    end
    
    for prime = 2, n do
        if p[prime] > 0 then
            for k = prime * 2, n, prime do
                p[k] = 0
            end
            local mul = 1
            local ones = 0
            while true do
                ones = 0
                mul = 1
                for i = 1, #a do
                    if a[i] % prime == 0 then
                        a[i] = a[i] // prime
                        mul = prime
                    end
                    if a[i] == 1 then
                        ones = ones + 1
                    end
                end
                if mul == 1 then
                    break
                end
                res = res * mul
            end
            if ones == #a then
                break
            end
        end
    end
    return res
end

function part2()

    first1, loop_len1 = get_repeating_coord(1)
    first2, loop_len2 = get_repeating_coord(2)
    first3, loop_len3 = get_repeating_coord(3)

    -- assuming each loop started with the first state

    -- solution a = { 186028, 286332, 96236 }
    local l = lcm({loop_len1, loop_len2, loop_len3})


    print(l)

end

part1()
part2()


