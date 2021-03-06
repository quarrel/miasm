from miasm2.expression.parser import str_to_expr
from miasm2.expression.expression import ExprInt, ExprId, ExprSlice, ExprMem, \
    ExprCond, ExprCompose, ExprOp, ExprAff, ExprLoc, LocKey

for expr_test in [ExprInt(0x12, 32),
                  ExprId('test', 32),
                  ExprLoc(LocKey(12), 32),
                  ExprSlice(ExprInt(0x10, 32), 0, 8),
                  ExprMem(ExprInt(0x10, 32), 32),
                  ExprCond(ExprInt(0x10, 32), ExprInt(0x11, 32), ExprInt(0x12, 32)),
                  ExprCompose(ExprInt(0x10, 16), ExprInt(0x11, 8), ExprInt(0x12, 8)),
                  ExprInt(0x11, 8) + ExprInt(0x12, 8),
                  ExprAff(ExprId('EAX', 32),  ExprInt(0x12, 32)),
                  ]:

    print 'Test: %s' % expr_test
    assert str_to_expr(repr(expr_test)) == expr_test
