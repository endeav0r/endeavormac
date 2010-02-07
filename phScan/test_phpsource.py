import phpsource

s = phpsource.PhpSource()
s.from_raw_text('echo "test";')
s.parse(in_php = True)
print s.items
print s.statements
