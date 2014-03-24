sources  = FileList["*.c"]
includes = FileList["*.h"]
file "dnsd" => (sources + includes) do
  sh "gcc -Wall -g -O0 -o dnsd #{sources.join(" ")}"
end

task :default => "dnsd"
