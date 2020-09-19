class Main
{
	constructor()
	{
		this.entityList = [
			new Player(),
		]

		this.CallStart()
	}

	CallStart()
	{
		for( let i = 0; i < this.entityList.length; ++i )
		{
			this.entityList[i].EntityStart()
		}
	}

	CallUpdate()
	{
		for( let i = 0; i < this.entityList.length; ++i )
		{
			this.entityList[i].EntityUpdate()
		}
	}

	CallDraw()
	{
		for( let i = 0; i < this.entityList.length; ++i )
		{
			this.entityList[i].EntityDraw()
		}
	}

	CallCollision()
	{
		for( let i = 0; i < this.entityList.length; ++i )
		{
			this.entityList[i].EntityCollision()
		}
	}

	CallDestroy()
	{
		for( let i = 0; i < this.entityList.length; ++i )
		{
			this.entityList[i].EntityDestroy()
		}
	}

}

const main = new Main()
const gfx = new Graphics()
setInterval( function()
{
	gfx.DrawRect( 0,0,960,540,"#000000" )
	main.CallUpdate()
	main.CallDraw()
},1000 / 30 )