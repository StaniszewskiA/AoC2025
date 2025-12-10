using JuMP
using GLPK

function parse_input(filename::String)::Vector{Tuple{Vector{Vector{Int}}, Vector{Int}}}
    machines = Tuple{Vector{Vector{Int}}, Vector{Int}}[]

    for line in eachline(filename)
        line = strip(line)
        isempty(line) && continue

        button_matches = collect(eachmatch(r"\((.*?)\)", line))
        buttons = [parse.(Int, split(bm.captures[1], ",")) for bm in button_matches]

        joltage_str = match(r"\{(.*?)\}", line)
        targets = parse.(Int, split(joltage_str.captures[1], ","))

        push!(machines, (buttons, targets))
    end

    return machines  
end

function min_button_presses(buttons::Vector{Vector{Int}}, targets::Vector{Int})::Int
    n_buttons::Int = length(buttons)
    n_counters::Int = length(targets)

    model = Model(GLPK.Optimizer)
    @variable(model, x[1:n_buttons] >= 0, Int)

    for j in 1:n_counters
        @constraint(model, sum(x[i] for i in 1:n_buttons if j - 1 in buttons[i]) == targets[j])
    end

    @objective(model, Min, sum(x))
    optimize!(model)

    if termination_status(model) == MOI.OPTIMAL
        return Int(objective_value(model))
    else
        error("No solution found for a machine")
    end
end

function solve_day10b(filename::String)::Nothing
    machines = parse_input(filename)
    println("Res: ", sum(min_button_presses(buttons, targets) for (buttons, targets) in machines))
    return nothing
end

solve("inputs/day10a.txt")
