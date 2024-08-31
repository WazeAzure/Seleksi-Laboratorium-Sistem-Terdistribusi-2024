(lambda: 
    (
        (lambda code: exit(code))(1) if not callable(getattr(__builtins__, 'compile', None)) or getattr(__builtins__.compile, '__module__', '') != 'builtins' or getattr(__builtins__.compile, '__name__', '') != 'compile' else None))()