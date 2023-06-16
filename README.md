# Pokemon
物件導向 Project3

# 實作功能

- Load data (Pokémon, moves, and game)		10%
- Output							10%
- 3 vs 3 game						10%
- PP (Power Point)					10%
- Status condition (Paralysis, Burn, and Poison)	10%
- Physical or special move				   5%
- Speed (who go first)				  	   5%
- Type effectiveness					   5%
- Swap Pokémon					   5%
- Potion							   5%
- Victory or defeat					   5%
- GUI							15%
- Audio							  5%
- Accuracy 				  		  5%
- Critical hit 				  		  5%


# 設定

程式有兩種運行方式：一般模式、command file模式。打開選單後可以對運行模式進行調整。

1. 一般模式
    > 當`Use Command File`未被勾選時為一般模式。你可以選擇pokemonLib、moveLib、gameData、testing mode。
    > 
    > testing mode開啟時，paralyzed必定使之無法攻擊，Accuracy必為100%，暴擊率為0。

2. Command File模式
    > 當`Use Command File`被勾選時為Command File模式，你可以選擇要執行的command file

# 遊玩畫面

畫面右下角有四個按鈕，分別是`Battle`、`Pokemon`、`Bag`、`Run`。除了`Run`是點一次就會逃跑外，其他三個按鈕都是點一次切換到一個選單、再點一次切回文字顯示框。

點`Battle`會進入技能選單、`Pokemon`會進入寶可夢選單（選擇要換成的寶可夢）、`Bag`會進入道具（補藥）選單。在三個顯示框中右鍵點擊按鈕都能獲得更詳細的說明。

當你在`Bag`中選了一個道具，此時會要你選擇用在哪個寶可夢上，點`Back`可以取消使用道具。

# Media Source

- [pokemon pixel art](https://pokemondb.net/sprites)
- [pokemon](https://www.pokemon.com/us/pokedex)
- [item](https://pokemondb.net/item/all)
- [type](https://pokemongo.fandom.com/zh/wiki/%E5%B1%AC%E6%80%A7?variant=zh-tw)
- [background](https://www.deviantart.com/phoenixoflight92/art/Pokemon-X-and-Y-battle-background-10-490608175)
- [logo](https://commons.wikimedia.org/wiki/File:International_Pok%C3%A9mon_logo.svg)
- Move Type Icon
  - [Special Move](https://www.deviantart.com/jormxdos/art/Special-Move-Icon-934387511)
  - [Status Move](https://www.deviantart.com/jormxdos/art/Status-Move-Icon-934387530)
  - [Physical Move](https://www.deviantart.com/jormxdos/art/Physical-Move-Icon-934387518)
- Music
  - [Damage](https://soundeffects.fandom.com/wiki/Pok%C3%A9mon_Damage)
  - [Heal](https://soundeffects.fandom.com/wiki/Pok%C3%A9mon_Healing_Sound)
  - [BGM](https://www.youtube.com/watch?v=Cp-waxoLel8&t=6s)

# Move List

<https://pokemondb.net/move/all>

# 能力值計算機

取等級50時各數值的最大值

<http://mpokemon.com/cht/bw/statcalc.php>