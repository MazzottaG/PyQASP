input = """
 produced_by(pasta   , barilla ,saiwa). 
 produced_by(tomatoes, frutto  ,barilla). 
 produced_by(wine    , barilla ,0). 
 produced_by(bread   , saiwa   ,panino). 
 produced_by(beer    , heineken,budweiser). 

 controlled_by(frutto   , barilla  ,saiwa,0).
 controlled_by(budweiser, heineken ,0    ,0). 
 controlled_by(heineken , saiwa    ,0    ,0).
%controlled_by(saiwa    , budweiser,0    ,0). 
"""
output = """
{controlled_by(budweiser,heineken,0,0), controlled_by(frutto,barilla,saiwa,0), controlled_by(heineken,saiwa,0,0), produced_by(beer,heineken,budweiser), produced_by(bread,saiwa,panino), produced_by(pasta,barilla,saiwa), produced_by(tomatoes,frutto,barilla), produced_by(wine,barilla,0)}
"""
